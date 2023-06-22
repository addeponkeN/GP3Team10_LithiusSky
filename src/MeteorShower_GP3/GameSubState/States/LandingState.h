// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSubState.h"
#include "LandingState.generated.h"

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API ALandingState : public ABaseSubState
{
	GENERATED_BODY()

public:
	ALandingState();

protected:
	virtual void BeginPlay() override;

public:
	virtual void StartEnterState() override;
	virtual void OnEnteredState() override;
	virtual void StartExitState() override;
	virtual void OnExitedState() override;
};
