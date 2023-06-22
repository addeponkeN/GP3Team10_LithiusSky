// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Widgets/CharacterSlider.h"
#include "DebugUi.generated.h"

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API UDebugUi : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	UFUNCTION()
	void OnOpened();

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UButton* BtTimerToggle;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* LbTimerText;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	USlider* SlMeteorTimer;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UCharacterSlider* SlSensitivity;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* LbSensText;

	UFUNCTION()
	void SlMeteorTimer_ValueChanged(float SliderValue);

	UFUNCTION()
	void SlMeteorTimer_SetValue(AMeteorTimer* MeteorTimer);
	
	UFUNCTION()
	void SlSensitivity_ValueChanged(float SliderValue);

	UFUNCTION()
	void BtTimerToggle_Clicked();
};
