#include "CheckpointManager.h"

#include "Checkpoint.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GigaCheckpoint.h"
#include "../MeteorShower_GP3.h"
#include "Kismet/GameplayStatics.h"
#include "MeteorShower_GP3/MSGameInstance.h"
#include "MeteorShower_GP3/MSGameMode.h"
#include "MeteorShower_GP3/DataAssets/CheckpointManagerDataAsset.h"
#include "MeteorShower_GP3/GameSubState/States/PlayingState.h"
#include "MeteorShower_GP3/GameSubState/States/RestartState.h"

UCheckpointManager::UCheckpointManager()
{
	PrimaryComponentTick.bCanEverTick = false;
	const auto CollisionProfileName = TEXT("Checkpoint");
	SetCollisionProfileName(CollisionProfileName);
	SphereRadius = 80.f;
}

void UCheckpointManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FallbackCheckpoints;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("fallbackgigacheckpoint")), FallbackCheckpoints);

	if (FallbackCheckpoints.Num() > 0)
	{
		AGigaCheckpoint* TaggedFallbackGigaCheckpoint = Cast<AGigaCheckpoint>(FallbackCheckpoints[0]);
		SetFallbackGigaCheckpoint(TaggedFallbackGigaCheckpoint);
		LOGI("Set 'FallbackGigaCheckpoint'");
	}
	else
	{
		LOGW("Could not find a 'FallbackGigaCheckpoint' in the level");
	}

	SetCollisionProfileName("Checkpoint");

	OnComponentBeginOverlap.AddDynamic(this, &UCheckpointManager::OnSphereBeginOverlap);

	if (!Settings)
	{
		LOGW("'Character BP > CheckpointCollider' is missing 'Settings' (CheckpointManagerDataAsset)")
	}
	else
	{
		const ACharacter* Character = Cast<ACharacter>(GetOwner());
		if (!Character)
		{
			LOGW("CheckpointManager no owner");
			return;
		}

		const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		if (PlayerController)
		{
			UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
			if (InputComponent)
			{
				InputComponent->BindAction(
					Settings->QuickRestartAction, ETriggerEvent::Triggered, this, &UCheckpointManager::QuickRestart);
				InputComponent->BindAction(
					Settings->RestartAction, ETriggerEvent::Triggered, this, &UCheckpointManager::GigaRestart);
			}
		}
	}
}

void UCheckpointManager::QuickRestart()
{
	if (!CurrentCheckpoint)
	{
		return;
	}

	if (!CanRespawn())
	{
		return;
	}

	OnRestart(false);
}

void UCheckpointManager::GigaRestart()
{
	if (!GigaCheckpoint && !FallbackGigaCheckpoint)
	{
		return;
	}

	if (!CanRespawn())
	{
		return;
	}

	OnRestart(true);
}

void UCheckpointManager::OnRestart(bool IsGigaRestart)
{
	bCanRespawn = false;
	const AMSGameMode* GameMode = AMSGameMode::GetGameMode(GetWorld());
	ARestartState* RestartState = GameMode->StateManager->SetState<ARestartState>();
	RestartState->SetIsGigaRestart(IsGigaRestart);
	GetWorld()->GetTimerManager()
	          .SetTimer(CooldownTimerHandle, this, &UCheckpointManager::ClearRespawnCooldown, RespawnCooldown);
}

bool UCheckpointManager::CanRespawn()
{
	if (!bCanRespawn)
		return false;
	const AMSGameMode* GameMode = AMSGameMode::GetGameMode(GetWorld());
	const bool bIsCurrentState_PlayingState = GameMode->StateManager->CurrentState->StateType ==
		ESubGameStates::Playing;
	return bIsCurrentState_PlayingState;
}

void UCheckpointManager::ClearRespawnCooldown()
{
	bCanRespawn = true;
}

void UCheckpointManager::ClearCurrentCheckpoint()
{
	if (FallbackCheckpoint)
	{
		SetCheckpoint(FallbackCheckpoint);
		// CurrentCheckpoint = FallbackCheckpoint;
		LOGF("CLEARED CHECKPOINT - IS NOW '%s'", *CurrentCheckpoint->GetName())
	}
}

void UCheckpointManager::RespawnAtCheckpoint()
{
	if (!CurrentCheckpoint)
	{
		LOGE("Could not respawn at checkpoint - no active checkpoint");
		return;
	}

	UMSGameInstance* GameInstance = Cast<UMSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (!GameInstance)
	{
		LOGE("Failed to get game instance (?????)")
		return;
	}

	GameInstance->RestartFromCheckpoint();
}

void UCheckpointManager::SetCheckpoint(ACheckpoint* TargetCheckpoint)
{
	if (!TargetCheckpoint)
	{
		LOGE("Failed to set Checkpoint - null");
		return;
	}

	if (!FallbackCheckpoint)
	{
		FallbackCheckpoint = TargetCheckpoint;
	}

	if(CurrentCheckpoint)
	{
		CurrentCheckpoint->SetIsCurrent(false);
	}

	CurrentCheckpoint = TargetCheckpoint;
	CurrentCheckpoint->SetIsCurrent(true);
	
	LOGF("Set Checkpoint: '%s'", *CurrentCheckpoint->GetName());
	OnActivatedCheckpointEvent.Broadcast(CurrentCheckpoint);

	UMSGameInstance* GameInstance =
		Cast<UMSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (!GameInstance)
	{
		LOGE("Failed to get game instance (?????)")
		return;
	}

	GameInstance->SaveCheckpointState(CurrentCheckpoint);
}

void UCheckpointManager::SetGigaCheckpoint(AGigaCheckpoint* TargetGigaCheckpoint)
{
	// set giga checkpoint
	
	if (GigaCheckpoint)
	{
		// LOGW("GigaCheckpoint already set!!");
		return;
	}

	if(FallbackGigaCheckpoint == TargetGigaCheckpoint)
	{
		return;
	}
	
	GigaCheckpoint = TargetGigaCheckpoint;

	LOGF("Set GIGACheckpoint: '%s'", *GigaCheckpoint->GetName());

	UMSGameInstance* GameInstance = GetWorld()->GetGameInstance<UMSGameInstance>();
	if (GameInstance)
	{
		GameInstance->SaveGameState(GigaCheckpoint);
	}
}

void UCheckpointManager::SetFallbackGigaCheckpoint(AGigaCheckpoint* TargetGigaCheckpoint)
{
	// set giga checkpoint
	if (FallbackGigaCheckpoint)
	{
		LOGW("FallbackGIGACheckpoint already set!!");
		return;
	}
	FallbackGigaCheckpoint = TargetGigaCheckpoint;

	LOGF("Set FallbackGIGACheckpoint: '%s'", *FallbackGigaCheckpoint->GetName());

	UMSGameInstance* GameInstance = GetWorld()->GetGameInstance<UMSGameInstance>();
	if (GameInstance)
	{
		GameInstance->SaveGameState(FallbackGigaCheckpoint);
	}
}

void UCheckpointManager::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                              const FHitResult& SweepResult)
{
	// LOGIF("collided with thing !! >  '%s'", *OtherActor->GetName());
	if (CurrentCheckpoint && CurrentCheckpoint == OtherActor)
	{
		LOGF("Did not set new checkpoint - checkpoint was the same as current: '%s'", *OtherActor->GetName());
		return;
	}

	if (AGigaCheckpoint* OtherGigaCheckpoint = Cast<AGigaCheckpoint>(OtherActor))
	{
		// LOG("was giga checkpoint");
		if (!GigaCheckpoint)
		{
			// LOG("set giga checkpoint");
			SetGigaCheckpoint(OtherGigaCheckpoint);
		}
		return;
	}
	else
	{
		// LOG("not giga checkpoint");
	}

	if (ACheckpoint* OtherCheckpoint = Cast<ACheckpoint>(OtherActor))
	{
		// LOG("was & set checkpoint");
		SetCheckpoint(OtherCheckpoint);
	}
}
