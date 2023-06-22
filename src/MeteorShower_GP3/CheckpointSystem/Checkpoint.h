// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsSetCurrent, bool, IsCurrent);

UCLASS()
class METEORSHOWER_GP3_API ACheckpoint : public AActor
{
	GENERATED_BODY()

public:
	ACheckpoint();

protected:
	virtual void BeginPlay() override;
	

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnIsSetCurrent OnIsSetCurrent;
	
	UFUNCTION(BlueprintCallable)
	void SetIsCurrent(bool IsCurrent);
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	UBoxComponent* Area;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* RespawnLocation;

	UPROPERTY(EditAnywhere)
	UArrowComponent* ForwardArrow;
};
