
#pragma once

#include "CoreMinimal.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

#include "CharacterSlider.generated.h"

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API UCharacterSlider : public USlider
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetSliderFromCharacterSensitivity();

	UPROPERTY()
	UTextBlock* Test;
};


