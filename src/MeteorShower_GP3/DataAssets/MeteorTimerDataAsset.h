// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeteorTimerDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API UMeteorTimerDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="CameraShake")
	float SecondsLeftWhenToShake{10.f};

	UPROPERTY(EditAnywhere, Category="CameraShake")
	float ShakeAmount{1.f};

	UPROPERTY(EditAnywhere, Category="WhiteScreen")
	float SecondsLeftWhenToFade{5.f};

	UPROPERTY(EditAnywhere, Category="Lighting")
	FLinearColor DirectionalLightColor{FLinearColor(1.f, 0.f, 0.f, 1.f)};
};
