// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "GameMenuUi.generated.h"

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API UGameMenuUi : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UButton* BtResume;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UButton* BtRewind;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UButton* BtQuit;

	UFUNCTION()
	void BtResume_Clicked();

	UFUNCTION()
	void BtRewind_Clicked();
	
	UFUNCTION()
	void BtQuit_Clicked();
	
};
