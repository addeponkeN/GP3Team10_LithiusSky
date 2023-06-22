// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Sound/SoundCue.h"
#include "CharacterSettings.generated.h"

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API UCharacterSettings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,Category = SFX)
	USoundCue* DashSoundCue;

	UPROPERTY(EditAnywhere,Category = SFX)
	USoundBase* OnLandSound;
};
