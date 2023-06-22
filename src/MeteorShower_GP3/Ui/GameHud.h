// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Slider.h"
#include "GameHud.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API UGameHud : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;
	
public:

	UFUNCTION()
	void OnRestartAction();
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* HourglassImage;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* RestartImage;

	// UPROPERTY(EditAnywhere)
	// USlider* RestartSlider;

	// UPROPERTY(EditAnywhere)
	// UProgressBar* MeteorProgressBar;
	
};
