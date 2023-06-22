#include "ValueSlider.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

#define LOCTEXT_NAMESPACE "UMG"

void UValueSlider::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (Slider)
	{
		Slider->SetValue(SliderValue);
		Slider->OnValueChanged.AddDynamic(this, &UValueSlider::ValueChanged);
	}

	if (TBSliderValue)
	{
		SetSliderValue(SliderValue);
	}

	if (TBTitle)
	{
		TBTitle->SetText(FText::FromString(TitleText));
	}
}

void UValueSlider::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UValueSlider::Initialize()
{
	return Super::Initialize();
}

void UValueSlider::SetSliderValue(float Value)
{
	if (Slider)
	{
		Slider->SetValue(Value);
	}
	if (TBSliderValue)
	{
		TBSliderValue->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), Value)));
	}
}

void UValueSlider::ValueChanged(float Value)
{
	OnValueChanged.Broadcast(Value);
	SetSliderValue(Value);
}

#if WITH_EDITOR
const FText UValueSlider::GetPaletteCategory()
{
	return LOCTEXT("VSlider", "ValueSlider");
}
#endif


#undef LOCTEXT_NAMESPACE
