// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MSMenuGameMode.generated.h"

class UMenuCollectionDataAsset;
class AMSMainMenuHUD;
class UMenuBase;
class UMenuManager;

UCLASS()
class METEORSHOWER_GP3_API AMSMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void StartPlay() override;
	virtual void BeginPlay() override;

public:

	UPROPERTY()
	UMenuManager* MenuManager;

	UPROPERTY(EditAnywhere)
	UMenuCollectionDataAsset* MenuCollection;
	
	UFUNCTION(BlueprintCallable)
	void SetMenu(TSubclassOf<UMenuBase> MenuClass);
};
