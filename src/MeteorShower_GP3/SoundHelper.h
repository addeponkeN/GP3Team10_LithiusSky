// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SoundHelper.generated.h"

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API USoundHelper : public UObject
{
	GENERATED_BODY()
	
public:
	USoundHelper();
	
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static UAudioComponent* PlaySoundSFX(UObject* WorldContextObject, USoundBase* Sound, float VolumeMultiplier = 1.0f);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static UAudioComponent* PlayMusic(UObject* WorldContextObject, USoundBase* Sound);
	
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static UAudioComponent* PlaySoundSFX3D(UObject* WorldContextObject, USoundBase* Sound, const FVector& Location);
	
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static UAudioComponent* PlayDialogueSound(UObject* WorldContextObject, USoundBase* Sound);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static float GetMusicVolume(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static float GetSFXVolume(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static float GetDialogueVolume(UObject* WorldContextObject);

};
