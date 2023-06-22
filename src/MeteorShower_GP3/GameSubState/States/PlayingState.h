// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSubState.h"
#include "GameFramework/Actor.h"
#include "MeteorShower_GP3/MeteorShower_GP3Character.h"
#include "PlayingState.generated.h"

UCLASS()
class METEORSHOWER_GP3_API APlayingState : public ABaseSubState
{
	GENERATED_BODY()

public:
	APlayingState();

protected:
	virtual void BeginPlay() override;
	virtual void StartEnterState() override;
	virtual void OnEnteredState() override;
	virtual void OnExitedState() override;

public:
	UFUNCTION()
	void OnCharacterDied();

private:
	UPROPERTY()
	AMeteorShower_GP3Character* Character;
};
