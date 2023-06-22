// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundHelper.h"

#include "MSGameInstance.h"
#include "MSGameMode.h"
#include "Components/AudioComponent.h"
#include "DataAssets/GameSettingsDataAsset.h"
#include "Kismet/GameplayStatics.h"

USoundHelper::USoundHelper()
{
}

UAudioComponent* USoundHelper::PlaySoundSFX(UObject* WorldContextObject, USoundBase* Sound, float VolumeMultiplier)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	if (!Sound)
	{
		LOGW("invalid sfx sound")
		return nullptr;
	}
	return UGameplayStatics::SpawnSound2D(WorldContextObject, Sound,
	                                      GetSFXVolume(WorldContextObject) * VolumeMultiplier);
}

UAudioComponent* USoundHelper::PlayMusic(UObject* WorldContextObject, USoundBase* Sound)
{
	if (!Sound)
	{
		LOGW("invalid sfx sound")
		return nullptr;
	}
	UAudioComponent* MusicComponent = UGameplayStatics::SpawnSound2D(WorldContextObject, Sound,
	                                                                 GetMusicVolume(WorldContextObject));
	UWorld* World = WorldContextObject->GetWorld();
	if (World)
	{
		UMSGameInstance* GameInstance = World->GetGameInstance<UMSGameInstance>();
		if (GameInstance)
		{
			if (GameInstance->CurrentMusic && IsValid(GameInstance))
			{
				GameInstance->CurrentMusic->Stop();
			}
			GameInstance->CurrentMusic = MusicComponent;
		}
	}
	return MusicComponent;
}

UAudioComponent* USoundHelper::PlaySoundSFX3D(UObject* WorldContextObject, USoundBase* Sound, const FVector& Location)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	if (!Sound)
	{
		LOGW("invalid sfx 3d sound");
		return nullptr;
	}
	return UGameplayStatics::SpawnSoundAtLocation(WorldContextObject, Sound, Location, FRotator::ZeroRotator,
	                                              GetSFXVolume(WorldContextObject));
}

UAudioComponent* USoundHelper::PlayDialogueSound(UObject* WorldContextObject, USoundBase* Sound)
{
	if (!Sound)
	{
		LOGW("invalid dialogue sound")
		return nullptr;
	}

	auto GameMode = AMSGameMode::GetGameMode(WorldContextObject);
	if (!GameMode)
	{
		LOGW("no gamemode on play dialoguue")
		return nullptr;
	}

	auto CurrentDialogue = GameMode->CurrentDialogue;
	if ((CurrentDialogue && IsValid(CurrentDialogue)))
	{
		CurrentDialogue->Stop();
		LOG("STOPPED DIALOGEUAEUE");
	}

	const float DialogueVolume = GetDialogueVolume(WorldContextObject);
	GameMode->CurrentDialogue = UGameplayStatics::SpawnSound2D(WorldContextObject, Sound, DialogueVolume);
	return GameMode->CurrentDialogue;
}

float USoundHelper::GetMusicVolume(UObject* WorldContextObject)
{
	const UGameSettingsDataAsset* Settings = UMSGameInstance::GetGameSettings(WorldContextObject);
	return Settings->AudioSettings->MusicVolume * Settings->AudioSettings->MasterVolume;
}

float USoundHelper::GetSFXVolume(UObject* WorldContextObject)
{
	const UGameSettingsDataAsset* Settings = UMSGameInstance::GetGameSettings(WorldContextObject);
	return Settings->AudioSettings->SFXVolume * Settings->AudioSettings->MasterVolume;
}

float USoundHelper::GetDialogueVolume(UObject* WorldContextObject)
{
	const UGameSettingsDataAsset* Settings = UMSGameInstance::GetGameSettings(WorldContextObject);
	return Settings->AudioSettings->DialogueVolume * Settings->AudioSettings->MasterVolume;
}
