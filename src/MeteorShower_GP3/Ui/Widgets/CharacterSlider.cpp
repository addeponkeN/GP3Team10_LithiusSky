#include "CharacterSlider.h"

#include "MeteorShower_GP3/MSGameInstance.h"
#include "MeteorShower_GP3/DataAssets/GameSettingsDataAsset.h"

void UCharacterSlider::SetSliderFromCharacterSensitivity()
{
	if (UGameSettingsDataAsset* Settings = UMSGameInstance::GetGameSettings(GetWorld()))
	{
		constexpr float MinSens = 0.25f;
		constexpr float MaxSens = 2.f;
		// inverse lerp
		const float t = (Settings->MouseSensitivity - MinSens) / (MaxSens - MinSens);
		SetValue(t);
	}
}
