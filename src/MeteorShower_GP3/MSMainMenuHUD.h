// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MSMainMenuHUD.generated.h"

class UMenuManager;
/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API AMSMainMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	UPROPERTY()
	UMenuManager* MenuManager;
};
