// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Laser.generated.h"

UCLASS()
class METEORSHOWER_GP3_API ALaser : public AActor
{
	GENERATED_BODY()

public:
	ALaser();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	float LifeTime = 2.f;

	UPROPERTY(EditAnywhere)
	UArrowComponent* Arrow;

	UFUNCTION(BlueprintCallable)
	virtual void SetDestination(FVector const& Destination);

	UFUNCTION(BlueprintCallable)
	void DestroyLaser();

	UFUNCTION(BlueprintCallable)
	void SetLifeTime(float Time);

	FTimerHandle KillTimerHandle;
};
