// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSubState.h"
#include "VictoryState.generated.h"

class AMeteorShower_GP3Character;
/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API AVictoryState : public ABaseSubState
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	virtual void StartEnterState() override;
	virtual void OnEnteredState() override;
	virtual void StartExitState() override;
	virtual void OnExitedState() override;

private:
	UPROPERTY()
	AMeteorShower_GP3Character* Character; 
};
