// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CutsceneSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API UCutsceneSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void StartCutscene(TSubclassOf<UUserWidget> CutsceneWidget);
};
