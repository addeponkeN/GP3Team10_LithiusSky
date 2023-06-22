#include "CutsceneManager.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MeteorShower_GP3.h"
#include "MSGameInstance.h"
#include "MSGameMode.h"
#include "DataAssets/CutsceneManagerDataAsset.h"
#include "DataAssets/GameSettingsDataAsset.h"
#include "Ui/CutscenePlayer.h"

ACutsceneManager::ACutsceneManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACutsceneManager::BeginPlay()
{
	Super::BeginPlay();
	LOG("CUSCENEMANAGER BEGIN");

	// auto World = GetWorld();
	// if (World)
	// {
	// 	auto GameInstance = World->GetGameInstance<UMSGameInstance>();
	// 	if (GameInstance)
	// 	{
	// 		if (GameInstance->GameSettings->PlayCutscene)
	// 		{
	// 			PlayCutscene(IntroCutscene);
	// 		}
	// 	}
	// }

	auto Character = AMSGameMode::GetCharacter(GetWorld());
	if (Character)
	{
		AttachToCharacter(Character);
	}
}

void ACutsceneManager::AttachToCharacter(AMeteorShower_GP3Character* Target)
{
	//	todo: actual attachment
	APlayerController* PlayerController = Cast<APlayerController>(Target->GetController());
	if (PlayerController)
	{
		UEnhancedInputComponent* CharacterInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
		if (CharacterInputComponent)
		{
			CharacterInputComponent->BindAction(Settings->SkipAction, ETriggerEvent::Triggered, this,
			                           &ACutsceneManager::SkipCutscene);
			LOGI("attached cutscenemanger to cahbracter");
		}
	}
}

bool ACutsceneManager::PlayCutscene(TSubclassOf<UUserWidget> CutsceneWidgetClass)
{
	// auto GameSettings = UMSGameInstance::GetGameSettings(GetWorld());
	// if(GameSettings && GameSettings->bSkipCutscene)
	// {
	// 	return false;
	// }
	// if (CurrentCutscene)
	// {
	// EndCutscene();
	// }

	LOGF("Playing Cutscene: %s", *CutsceneWidgetClass.Get()->GetName());
	auto PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		CurrentCutscene = CreateWidget<UCutscenePlayer>(PlayerController, CutsceneWidgetClass);
		CurrentCutscene->AddToViewport();
		CurrentCutscene->OnCutsceneEnd.AddDynamic(this, &ACutsceneManager::EndCutscene);
		AMSGameMode::SetCutsceneMode(this, true);
	}
	
	return true;
}

// OnEndCutscene*
void ACutsceneManager::EndCutscene()
{
	if (!CurrentCutscene)
	{
		CurrentCutscene = nullptr;
		return;
	}

	CurrentCutscene->OnCutsceneEnd.RemoveAll(this);
	CurrentCutscene->EndCutscene();
	OnEndCutscene();
	CurrentCutscene->RemoveFromParent();
	CurrentCutscene->ConditionalBeginDestroy();
	CurrentCutscene = nullptr;
	AMSGameMode::SetCutsceneMode(this, false);
}

void ACutsceneManager::SkipCutscene()
{
	if (!CurrentCutscene)
	{
		return;
	}
	CurrentCutscene->StopCutscene();
}
