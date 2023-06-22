// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// #include "GameFramework/SaveGame.h"
#include "SaveDataTypes/ActorSaveState.h"
#include "CheckpointSave.generated.h"

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API UCheckpointSave : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category=SaveGame)
	FString LevelName;

	UPROPERTY(VisibleAnywhere, Category=SaveGame)
	FTransform PlayerTransform;

	UPROPERTY(VisibleAnywhere, Category=SaveGame)
	int32 MeteorTimer;

	UPROPERTY(VisibleAnywhere, Category=SaveGame)
	TArray<FActorSaveState> ActorStates;

	UFUNCTION()
	void AddSaveState(const FActorSaveState& ActorState);
	FActorSaveState* GetActorData(const FString& ActorName);

private:
	UPROPERTY()
	TMap<FString, FActorSaveState> ActorMap; 
	
};
