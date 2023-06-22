// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GrappleSettings.generated.h"

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API UGrappleSettings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,Category= SFX)
	USoundBase* Shoot;
	UPROPERTY(EditAnywhere,Category= SFX)
	USoundBase* Reel;
	UPROPERTY(EditAnywhere,Category= SFX)
	USoundBase* Hit;
	UPROPERTY(EditAnywhere,Category = Animation)
	UAnimationAsset* Idle_Anim;
	UPROPERTY(EditAnywhere,Category = Animation)
	UAnimationAsset* Shoot_Anim;
	UPROPERTY(EditAnywhere,Category = Animation)
	UAnimationAsset* ReelIn_Anim;
	UPROPERTY(EditAnywhere,Category = Material)
	UMaterial* RopeMaterial;
	UPROPERTY(EditAnywhere,Category = Material)
	float MaterialTiling;
};
