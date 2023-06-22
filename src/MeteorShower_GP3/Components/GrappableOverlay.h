// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrappableOverlay.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLookEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class METEORSHOWER_GP3_API UGrappableOverlay : public UActorComponent
{
	GENERATED_BODY()
	
	// Sets default values for this component's properties
public:
	UGrappableOverlay();
	
	UPROPERTY(BlueprintAssignable, meta=(DisplayName = "OnLookBegin"))
	FOnLookEvent OnLookBegin_Implementable;
	UPROPERTY(BlueprintAssignable, meta=(DisplayName = "OnLookEnd"))
	FOnLookEvent OnLookEnd_Implementable;

	UFUNCTION()
	void OnLookBegin();
	UFUNCTION()
	void OnLookEnd();

	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<UStaticMeshComponent*> MeshComponents;
	
};
