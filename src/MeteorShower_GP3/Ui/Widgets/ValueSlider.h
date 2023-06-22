
#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "ValueSlider.generated.h"

class USlider;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnValueChangedEvent, float, Value);

UCLASS(meta=(DisplayName="Value Slider"))
class METEORSHOWER_GP3_API UValueSlider : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
public:

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category="Events")
	FOnValueChangedEvent OnValueChanged;

	UFUNCTION(BlueprintCallable, Category="Value Slider")
	void SetSliderValue(float Value);
	
	UFUNCTION()
	void ValueChanged(float Value);
	
	//	SLIDER
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Slider")
	USlider* Slider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SliderValue{0.5f};
	
	//	VALUE TEXT
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget), Category="SliderValue")
	UTextBlock* TBSliderValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SliderValue")
	FSlateFontInfo SliderValueFont;


	//	TITLE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget), Category="Title")
	UTextBlock* TBTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Title")
	FString TitleText{FString("SliderTitle")};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Title")
	FSlateFontInfo TitleFont;

};
