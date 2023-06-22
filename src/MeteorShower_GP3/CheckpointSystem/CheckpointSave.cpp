// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointSave.h"

void UCheckpointSave::AddSaveState(const FActorSaveState& ActorState)
{
	ActorStates.Add(ActorState);
	ActorMap.Add(ActorState.ActorName, ActorState);
}

FActorSaveState* UCheckpointSave::GetActorData(const FString& ActorName)
{
	return ActorMap.Find(ActorName);
}
