// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CheckpointManagerDataAsset.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API UCheckpointManagerDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* QuickRestartAction;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* RestartAction;

};
