// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveableInterface.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, BlueprintType)
class USaveableInterface : public UInterface
{
	GENERATED_BODY()
};

class METEORSHOWER_GP3_API ISaveableInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Saveable")
	bool Save();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Saveable")
	bool Load();
};
