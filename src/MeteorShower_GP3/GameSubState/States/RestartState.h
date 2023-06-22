// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSubState.h"
#include "RestartState.generated.h"

class AMeteorShower_GP3Character;
class AMSGameMode;
/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API ARestartState : public ABaseSubState
{
	GENERATED_BODY()
public:
	ARestartState();

protected:
	virtual void BeginPlay() override;

public:
	virtual void StartEnterState() override;
	virtual void OnEnteredState() override;
	virtual void StartExitState() override;
	virtual void OnExitedState() override;
	void SetIsGigaRestart(bool bIsGigaRestart);

private:

	bool bGigaRestart;
	
	UPROPERTY()
	AMSGameMode* GameMode;

	UPROPERTY()
	AMeteorShower_GP3Character* Character;

	FTimerHandle WaitTimerHandle;
};
