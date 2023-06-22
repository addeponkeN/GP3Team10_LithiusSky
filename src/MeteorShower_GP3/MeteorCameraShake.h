// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LegacyCameraShake.h"
#include "Camera/CameraShakeBase.h"
#include "MeteorCameraShake.generated.h"

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API UMeteorCameraShake : public ULegacyCameraShake
{
	GENERATED_BODY()
public:
	UMeteorCameraShake();
};
