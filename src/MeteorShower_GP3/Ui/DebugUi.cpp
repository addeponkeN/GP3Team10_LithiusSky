// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugUi.h"

#include "MeteorShower_GP3/MSGameInstance.h"
#include "MeteorShower_GP3/MSGameMode.h"
#include "MeteorShower_GP3/DataAssets/GameSettingsDataAsset.h"

void UDebugUi::NativeConstruct()
{
	Super::NativeConstruct();

	//	init sensitivity slider
	SlSensitivity->OnValueChanged.AddDynamic(this, &UDebugUi::SlSensitivity_ValueChanged);


	//	init meteor timer slider
	SlMeteorTimer->OnValueChanged.AddDynamic(this, &UDebugUi::SlMeteorTimer_ValueChanged);
	AMeteorTimer* MeteorTimer = GetWorld()->GetAuthGameMode<AMSGameMode>()->GetMeteorTimer();
	if (MeteorTimer)
	{
		MeteorTimer->OnTimerChangedEvent.AddDynamic(this, &UDebugUi::SlMeteorTimer_SetValue);
	}

	//	timer buttons
	BtTimerToggle->OnClicked.AddDynamic(this, &UDebugUi::BtTimerToggle_Clicked);
}

void UDebugUi::OnOpened()
{
	SlSensitivity->SetSliderFromCharacterSensitivity();
	SlSensitivity_ValueChanged(SlSensitivity->GetValue());
	AMeteorTimer* MeteorTimer = GetWorld()->GetAuthGameMode<AMSGameMode>()->GetMeteorTimer();
	if (MeteorTimer->bTimerEnabled)
	{
		LbTimerText->SetText(FText::FromString(TEXT("Stop Timer")));
	}
	else
	{
		LbTimerText->SetText(FText::FromString(TEXT("Start Timer")));
	}
}

void UDebugUi::BtTimerToggle_Clicked()
{
	AMeteorTimer* MeteorTimer = GetWorld()->GetAuthGameMode<AMSGameMode>()->GetMeteorTimer();
	if (MeteorTimer->bTimerEnabled)
	{
		MeteorTimer->PauseTimer();
		LbTimerText->SetText(FText::FromString(TEXT("Start Timer")));
	}
	else
	{
		MeteorTimer->StartTimer();
		LbTimerText->SetText(FText::FromString(TEXT("Stop Timer")));
	}
}

void UDebugUi::SlMeteorTimer_ValueChanged(float SliderValue)
{
	AMeteorTimer* MeteorTimer = GetWorld()->GetAuthGameMode<AMSGameMode>()->GetMeteorTimer();
	const float NewTimerValue = FMath::Lerp(0.f, MeteorTimer->MeteorImpactTime, SliderValue);

	MeteorTimer->SetTimer(NewTimerValue);
}

void UDebugUi::SlMeteorTimer_SetValue(AMeteorTimer* MeteorTimer)
{
	const auto TimerPercent = MeteorTimer->GetPercent();
	if (FMath::Abs(SlMeteorTimer->GetValue() - TimerPercent) > 0.001f)
	{
		SlMeteorTimer->SetValue(TimerPercent);
	}
}

void UDebugUi::SlSensitivity_ValueChanged(float SliderValue)
{
	if (UGameSettingsDataAsset* Settings = UMSGameInstance::GetGameSettings(GetWorld()))
	{
		Settings->MouseSensitivity = FMath::Lerp(0.25f, 2.f, SliderValue);
		LbSensText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), Settings->MouseSensitivity)));
	}
}
