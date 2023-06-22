// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuBase.h"
#include "SettingsMenu.generated.h"

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API USettingsMenu : public UMenuBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
};
