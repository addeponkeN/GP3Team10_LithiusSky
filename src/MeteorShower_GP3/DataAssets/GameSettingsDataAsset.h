// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameSettingsDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FGameStateSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float EnterStateTime{0.f};

	UPROPERTY(EditAnywhere)
	float EnterFadeTime{0.f};

	UPROPERTY(EditAnywhere)
	float ExitStateTime{0.f};

	UPROPERTY(EditAnywhere)
	float ExitFadeTime{0.f};
};

UCLASS(BlueprintType)
class METEORSHOWER_GP3_API UGameAudioSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MasterVolume{1.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MusicVolume{0.5f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SFXVolume{0.5f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DialogueVolume{0.5f};
};

/**
 * 
 */
UCLASS(Blueprintable)
class METEORSHOWER_GP3_API UGameSettingsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameSettings")
	bool bFOVEnabled{true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameSettings")
	float MouseSensitivity{0.5f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameSettings")
	bool bAutoRun{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameSettings")
	bool bSkipCutscene{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AudioSettings")
	UGameAudioSettings* AudioSettings;

	UPROPERTY(EditAnywhere, Category="StateSettings")
	FGameStateSettings Landing;

	UPROPERTY(EditAnywhere, Category="StateSettings")
	FGameStateSettings Playing;

	UPROPERTY(EditAnywhere, Category="StateSettings")
	FGameStateSettings Dead;

	//	SETTERS
	// void SetMasterVolume(const float Value)
	// {
	// 	AudioSettings.MasterVolume = Value;
	// }
	//
	// void SetSFXVolume(const float Value)
	// {
	// 	AudioSettings.SFXVolume = Value;
	// }
	//
	// void SetMusicVolume(const float Value)
	// {
	// 	AudioSettings.MusicVolume = Value;
	// }
	//
	// void SetDialogueVolume(const float Value)
	// {
	// 	AudioSettings.DialogueVolume = Value;
	// }
};
