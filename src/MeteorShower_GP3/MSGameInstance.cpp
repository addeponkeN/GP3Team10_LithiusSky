#include "MSGameInstance.h"

#include "MeteorShower_GP3.h"
#include "MSGameMode.h"
#include "CheckpointSystem/Checkpoint.h"

// UMSGameInstance* UMSGameInstance::Instance = nullptr;

void UMSGameInstance::Init()
{
	Super::Init();
}

void UMSGameInstance::AddSaveable_Implementation(USaveableComponent* ComponentToAdd)
{
	Saveables.Add(ComponentToAdd);
}

void UMSGameInstance::SaveTo(UCheckpointSave* Save, ACheckpoint* Checkpoint)
{
	if (!Save)
	{
		LOG("SaveTo() Save parameter was null");
		return;
	}

	FTransform RespawnTransform;

	if (Checkpoint->RespawnLocation)
	{
		RespawnTransform = Checkpoint->RespawnLocation->GetComponentTransform();
	}
	else
	{
		RespawnTransform = AMSGameMode::GetCharacter(GetWorld())->GetActorTransform();
	}


	Save->PlayerTransform = RespawnTransform;
	// Save->LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	// FVector PlayerLocation(10.f, 20.f, 30.f);
	// FArchive Ar;
	// Ar << PlayerLocation;

	for (const USaveableComponent* Saveable : Saveables)
	{
		if (!Saveable)
			continue;
		FActorSaveState State;
		const AActor* SaveableOwner = Saveable->GetOwner();
		if (!SaveableOwner)
			continue;
		State.Transform = SaveableOwner->GetTransform();
		State.ActorName = SaveableOwner->GetName();
		State.State = 0;

		// FVector OwnerLocation = SaveableOwner->GetActorLocation();
		// Ar << OwnerLocation;

		Save->AddSaveState(State);
	}
}

void UMSGameInstance::LoadFrom(UCheckpointSave* Save)
{
	if (!Save)
	{
		LOGE("LoadFrom() Save parameter null")
		return;
	}

	AMeteorShower_GP3Character* Character = GetCharacter();
	APlayerController* CharacterController = Cast<APlayerController>(Character->GetController());

	Character->GetMesh1P()->SetPhysicsLinearVelocity(FVector::ZeroVector);
	Character->SetActorLocation(Save->PlayerTransform.GetLocation());
	CharacterController->SetControlRotation(Save->PlayerTransform.Rotator());

	for (const USaveableComponent* Saveable : Saveables)
	{
		AActor* SaveableOwner = Saveable->GetOwner();
		const FActorSaveState* State = Save->GetActorData(SaveableOwner->GetName());
		SaveableOwner->SetActorTransform(State->Transform);
	}
}

void UMSGameInstance::SaveCheckpointState(ACheckpoint* Checkpoint)
{
	if (!Checkpoint)
	{
		return;
	}

	if (CurrentSaveState)
	{
		CurrentSaveState->ConditionalBeginDestroy();
		CurrentSaveState = nullptr;
	}

	CurrentSaveState = NewObject<UCheckpointSave>();
	//Cast<UCheckpointSave>(UGameplayStatics::CreateSaveGameObject(UCheckpointSave::StaticClass()));

	SaveTo(CurrentSaveState, Checkpoint);
}

void UMSGameInstance::LoadCheckpointState()
{
	if (!CurrentSaveState)
	{
		LOGE("Load failed - no save data available")
		return;
	}

	LoadFrom(CurrentSaveState);
}

void UMSGameInstance::SaveGameState(ACheckpoint* Checkpoint)
{
	CurrentGigaSaveState = NewObject<UCheckpointSave>();
	//Cast<UCheckpointSave>(UGameplayStatics::CreateSaveGameObject(UCheckpointSave::StaticClass()));

	SaveTo(CurrentGigaSaveState, Checkpoint);
}

/**
 * @brief Load & Respawn at the GIGA checkpoint
 */
void UMSGameInstance::LoadGameState()
{
	if (!CurrentGigaSaveState)
	{
		LOGE("GigaLoad failed - no giga save data available")
		return;
	}

	const APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller->IsPaused())
	{
		AMSGameMode* GameMode = AMSGameMode::GetGameMode(GetWorld());
		GameMode->UnpauseGame();
	}

	LoadFrom(CurrentGigaSaveState);
}

void UMSGameInstance::RestartFromCheckpoint()
{
	if (!CurrentSaveState)
	{
		LOGE("Failed to RestartFromCheckpoint - no save data found")
		return;
	}

	LoadCheckpointState();
}

void UMSGameInstance::RestartFromGigaCheckpoint()
{
	if (!CurrentGigaSaveState)
	{
		LOGE("Failed to RestartFromGigaCheckpoint - no giga save data")
		return;
	}

	LoadGameState();
}

void UMSGameInstance::ResetCheckpoints()
{
	if (CurrentSaveState)
	{
		CurrentSaveState->ConditionalBeginDestroy();
		CurrentSaveState = nullptr;
	}

	if (CurrentGigaSaveState)
	{
		CurrentGigaSaveState->ConditionalBeginDestroy();
		CurrentGigaSaveState = nullptr;
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#pragma region DIRTY STATIC GETTERS

UGameSettingsDataAsset* UMSGameInstance::GetGameSettings(UObject* WorldContextObject)
{
	return GetGameInstance(WorldContextObject)->GameSettings;
}

UMSGameInstance* UMSGameInstance::GetGameInstance(UObject* WorldContextObject)
{
	return WorldContextObject->GetWorld()->GetGameInstance<UMSGameInstance>();
}

AMeteorShower_GP3Character* UMSGameInstance::GetCharacter()
{
	return Cast<AMeteorShower_GP3Character>(GetWorld()->GetFirstPlayerController()->GetCharacter());
}

#pragma endregion
