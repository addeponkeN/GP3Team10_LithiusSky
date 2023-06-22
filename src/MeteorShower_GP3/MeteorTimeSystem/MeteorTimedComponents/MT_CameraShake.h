// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeteorShower_GP3/MeteorCameraShake.h"
#include "MeteorShower_GP3/MeteorTimeSystem/MeteorTimed.h"
#include "MT_CameraShake.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class METEORSHOWER_GP3_API UMT_CameraShake : public UActorComponent, public IMeteorTimed
{
	GENERATED_BODY()

public:	
	UMT_CameraShake();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSetTime(AMeteorTimer* MeteorTimer) override;

private:
	UPROPERTY()
	UMeteorCameraShake* Shake;

	UPROPERTY()
	APlayerCameraManager* CameraManager;

	bool bShaking;
		
};
