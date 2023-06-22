// Copyright Epic Games, Inc. All Rights Reserved.

#include "MSGameMode.h"

#include "CutsceneManager.h"
#include "CutsceneSubsystem.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "LoopManager.h"
#include "MeteorShower_GP3.h"
#include "MeteorShower_GP3Character.h"
#include "MSGameInstance.h"
#include "Components/WidgetComponent.h"
#include "DataAssets/GameSettingsDataAsset.h"
#include "GameSubState/States/LandingState.h"
#include "GameSubState/States/VictoryState.h"
#include "Kismet/GameplayStatics.h"
#include "MeteorTimeSystem/MeteorTimer.h"
#include "UObject/ConstructorHelpers.h"

class AMeteorShower_GP3Character;

AMSGameMode::AMSGameMode() : Super()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	if (DefaultPlayerClass)
	{
		DefaultPawnClass = DefaultPlayerClass;
	}
	GameDebugUiClass = GetGameUiClass<UDebugUi>("W_DebugMenu");
	GameOverHudClass = GetGameUiClass<UGameOverHud>("W_GameOverHud");
	GameMenuUiClass = GetGameUiClass<UGameMenuUi>("W_GameMenu");
	SettingsMenuUiClass = GetGameUiClass<USettingsMenu>("W_InGameSettingsMenu");

	const FString BlueprintPath = TEXT("/Game/Blueprints/BP_MeteorTimerSystem.BP_MeteorTimerSystem_C");
	static ConstructorHelpers::FClassFinder<AMeteorTimer> GetMeteorTimerClass(*BlueprintPath);
	if (GetMeteorTimerClass.Succeeded())
	{
		MeteorTimerClass = GetMeteorTimerClass.Class;
	}
}

// UGameSettingsDataAsset* AMSGameMode::GetGameSettings(const UObject* WorldContextObject)
// {
// 	return GetGameMode(WorldContextObject)->GameSettings;
// }

//	this makes it so the character wont auto spawn when you start the game
//	spawning is manually done in LandingState.cpp instead
UClass* AMSGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	return nullptr;
}

void AMSGameMode::StartPlay()
{
	auto GameMode = AMSGameMode::GetGameMode(GetWorld());

	//	spawn player at the 'PlayerStart' location, if it exists
	UWorld* World = GetWorld();
	const TSubclassOf<APawn> DefaultPawn = GameMode->DefaultPawnClass;

	APlayerController* Controller = World->GetFirstPlayerController();
	const AActor* PlayerStart = GameMode->FindPlayerStart(Controller);
	const FTransform SpawnTransform = PlayerStart ? PlayerStart->GetTransform() : FTransform::Identity;

	//	this is called now
	AMeteorShower_GP3Character* Character =
		World->SpawnActorDeferred<AMeteorShower_GP3Character>(DefaultPawn, SpawnTransform);
	Controller->AttachToActor(Character, FAttachmentTransformRules::KeepRelativeTransform, TEXT("PlayerController"));
	Controller->Possess(Character);
	Controller->SetInputMode(FInputModeGameOnly());
	Controller->SetShowMouseCursor(false);
	Character->SetInputEnabled(false);
	Character->FinishSpawning(SpawnTransform);

	InitMenus();

	StateManager = GetWorld()->SpawnActor<AGameSubStateManager>();

	Super::StartPlay();
}

void AMSGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bScoreTimer)
	{
		HighScoreTimer += DeltaSeconds;
	}
}

void AMSGameMode::BeginPlay()
{
	Super::BeginPlay();

	LOGI("ENTERED GAME");

	PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	// 	auto World = GetWorld();
	// 	if (World)
	// 	{
	// 		auto GameInstance = World->GetGameInstance<UMSGameInstance>();
	// 		if (GameInstance)
	// 		{
	// #if UE_EDITOR
	// 			if (GameInstance->GameSettings->PlayCutscene)
	// 			{
	// 				auto CutsceneSystem = GetWorld()->GetSubsystem<UCutsceneSubsystem>();
	// 				CutsceneSystem->StartCutscene();
	// 			}
	// 			else
	// 			{
	// 				StateManager->SetState<ALandingState>();
	// 			}
	// #else

	CutsceneManager = Cast<ACutsceneManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ACutsceneManager::StaticClass()));
	if (!CutsceneManager)
	{
		StateManager->SetState<ALandingState>();
		SetCharacterInputEnabled_CPP(true);
		SetShowCharacterGameUi(true);
	}
	// #endif
	// }
	// }

	//	temporary
	if
	(PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(UiMappingContext, 1);
		}
		UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
		if (InputComp)
		{
			InputComp->BindAction(PauseAction, ETriggerEvent::Triggered, this, &AMSGameMode::TogglePause);
#if UE_EDITOR
			InputComp->BindAction(DebugMenuAction, ETriggerEvent::Triggered, this, &AMSGameMode::ToggleDebugMenu);
#endif
		}
	}

	else
	{
		LOGE("char null on tempy thingy");
	}

	// SetShowCharacterGameUi(false);
	// SetCharacterInputEnabled_CPP(false);
}

void AMSGameMode::InitMenus()
{
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();

	GameOverHud = GameOverHudClass
		              ? CreateWidget<UGameOverHud>(Controller, GameOverHudClass)
		              : CreateWidget<UGameOverHud>(Controller);
	if (GameOverHud)
	{
		GameOverHud->AddToViewport();
		GameOverHud->GetFaderBlack()->SetFadeOpacity(1.f);
	}

	if (GameMenuUiTemplate)
	{
		GameMenuUi = CreateWidget<UGameMenuUi>(Controller, GameMenuUiTemplate);
		GameMenuUi->Priority = 2;
		GameMenuUi->AddToViewport();
		GameMenuUi->SetVisibility(ESlateVisibility::Hidden);
	}

	if (GameDebugUiClass)
	{
		GameDebugMenuUi = CreateWidget<UDebugUi>(Controller, GameDebugUiClass);
		GameDebugMenuUi->AddToViewport();
		GameDebugMenuUi->SetVisibility(ESlateVisibility::Hidden);
	}

	if (SettingsMenuUiClass)
	{
		SettingsMenuUi = CreateWidget<USettingsMenu>(Controller, SettingsMenuUiClass);
		SettingsMenuUi->AddToViewport();
		SettingsMenuUi->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMSGameMode::ToggleDebugMenu()
{
	// LOG("toggled debug menu");
	if (GameDebugMenuUi->GetVisibility() == ESlateVisibility::Hidden)
	{
		GameDebugMenuUi->SetVisibility(ESlateVisibility::Visible);
		GameDebugMenuUi->OnOpened();
		PlayerController->SetShowMouseCursor(true);
		GetCharacter(GetWorld())->DisableInput(PlayerController);
	}
	else
	{
		GameDebugMenuUi->SetVisibility(ESlateVisibility::Hidden);
		PlayerController->SetShowMouseCursor(false);
		GetCharacter(GetWorld())->EnableInput(PlayerController);
	}
}

void AMSGameMode::TogglePause()
{
	if (PlayerController->IsPaused())
	{
		UnpauseGame();
	}
	else
	{
		PauseGame();
	}
}

void AMSGameMode::PauseGame()
{
	if(bCutsceneMode)
	{
		return;
	}
	
	if (GameMenuUi)
	{
		GameMenuUi->SetVisibility(ESlateVisibility::Visible);
		PlayerController->SetPause(true);
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PauseHighScoreTimer();
		LOGI("Paused");
	}
	OnGamePaused.Broadcast(true);
}

void AMSGameMode::UnpauseGame()
{
	//	giga bad stuff
	//	if pause menu is visible when trying to unpause(aka pressing esc/p), hide settings menu instead
	if (SettingsMenuUi->GetVisibility() == ESlateVisibility::Visible)
	{
		SetShowSettingsMenu(false);
		return;
	}

	if (GameMenuUi)
	{
		GameMenuUi->SetVisibility(ESlateVisibility::Hidden);
		PlayerController->SetPause(false);
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
		ResumeHighScoreTimer();
		LOGI("Unpaused");
	}

	OnGamePaused.Broadcast(false);
}

void AMSGameMode::SetShowSettingsMenu(bool bShow)
{
	if (bShow)
	{
		SettingsMenuUi->SetVisibility(ESlateVisibility::Visible);
		GameMenuUi->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SettingsMenuUi->SetVisibility(ESlateVisibility::Hidden);
		GameMenuUi->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMSGameMode::SetShowCharacterGameUi(bool bShow)
{
	AMeteorShower_GP3Character* Character = GetCharacter(GetWorld());
	if (Character)
	{
		if (bShow)
		{
			Character->GameHud->SetVisibility(ESlateVisibility::Visible);
			LOG("SHOW ui");
		}
		else
		{
			Character->GameHud->SetVisibility(ESlateVisibility::Hidden);
			LOG("HIDE ui");
		}
	}
	else
	{
		LOG("SET UI FAILED");
	}
}

void AMSGameMode::SetCharacterInputEnabled_CPP(bool IsEnabled)
{
	AMeteorShower_GP3Character* Character = GetCharacter(GetWorld());

	if (Character)
	{
		Character->SetInputEnabled(IsEnabled);
	}
}

AMeteorTimer* AMSGameMode::GetMeteorTimer()
{
	if (!MeteorTimer)
	{
		if (!MeteorTimerClass)
			MeteorTimerClass = AMeteorTimer::StaticClass();
		MeteorTimer = GetWorld()->SpawnActor<AMeteorTimer>(MeteorTimerClass);
	}
	return MeteorTimer;
}

ALoopManager* AMSGameMode::GetLoopManager()
{
	if (!LoopManager)
	{
		LoopManager = NewObject<ALoopManager>();
	}
	return LoopManager;
}

float AMSGameMode::GetHighScoreTimer() const
{
	return HighScoreTimer;
}

void AMSGameMode::SetGameStateLanding()
{
	StateManager->SetState<ALandingState>();
}

void AMSGameMode::SetGameStateVictory()
{
	StateManager->SetState<AVictoryState>();
}

#pragma region DIRTY STATIC GETTERS

AMSGameMode* AMSGameMode::GetGameMode(const UObject* WorldContextObject)
{
	AMSGameMode* MeteorGameMode = Cast<AMSGameMode>(WorldContextObject->GetWorld()->GetAuthGameMode());
	return MeteorGameMode;
}

AMeteorTimer* AMSGameMode::GetMeteorTimerStatic(const UObject* WorldContextObject)
{
	return GetGameMode(WorldContextObject)->GetMeteorTimer();
}

AMSGameMode* AMSGameMode::GetGameModeBad(const UObject* WorldContextObject)
{
	AMSGameMode* MeteorGameMode = Cast<AMSGameMode>(WorldContextObject->GetWorld()->GetAuthGameMode());
	return MeteorGameMode;
}

AMeteorShower_GP3Character* AMSGameMode::GetCharacter(const UObject* WorldContextObject)
{
	AMeteorShower_GP3Character* Character =
		Cast<AMeteorShower_GP3Character>(
			WorldContextObject->GetWorld()->GetFirstPlayerController()->GetCharacter());
	return Character;
}

void AMSGameMode::SetCutsceneMode(const UObject* WorldContextObject, bool bIsPlayCutscene)
{
	auto GameMode = GetGameMode(WorldContextObject);
	if (!GameMode)
	{
		LOGW("failed to get gamemode - SetCutsceneMode");
		return;
	}

	// APlayerController* Controller = WorldContextObject->GetWorld()->GetFirstPlayerController();
	GameMode->bCutsceneMode = bIsPlayCutscene;
	
	if (bIsPlayCutscene)
	{
		// GameMode->MeteorTimer->PauseTimer();
		// auto Character = GetCharacter(WorldContextObject);
		// Controller->SetInputMode(FInputModeGameOnly());
	}
	else
	{
		// GameMode->MeteorTimer->StartTimer();
		// GameMode->StateManager->SetState<ALandingState>();
		// Controller->SetInputMode(FInputModeGameOnly());
		// GameMode->SetShowCharacterGameUi(true);
		// GameMode->SetCharacterInputEnabled_CPP(true);
	}

	GameMode->SetShowCharacterGameUi(!bIsPlayCutscene);
	GameMode->SetCharacterInputEnabled_CPP(!bIsPlayCutscene);
	GameMode->OnGameSetCutsceneMode.Broadcast(bIsPlayCutscene);

	LOGF("CUTSCENE MODE: %i", bIsPlayCutscene);
}

void AMSGameMode::SetCharacterInputEnabled(UObject* WorldContextObject, bool IsEnabled)
{
	auto world = WorldContextObject->GetWorld();
	if (!world)
	{
		LOGE("SetCharacterInputEnabled() -> Invalid WorldContext");
		return;
	}

	AMSGameMode* GameMode = world->GetAuthGameMode<AMSGameMode>();

	if (GameMode)
	{
		GameMode->SetCharacterInputEnabled_CPP(IsEnabled);
	}
}

#pragma endregion
