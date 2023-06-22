#include "MT_WhiteScreenFade.h"

#include "MeteorShower_GP3/MSGameMode.h"
#include "MeteorShower_GP3/Util/Easings.h"

UMT_WhiteScreenFade::UMT_WhiteScreenFade()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMT_WhiteScreenFade::BeginPlay()
{
	Super::BeginPlay();
	auto GameMode = AMSGameMode::GetGameMode(GetWorld());
	if (GameMode)
	{
		Fader = GameMode->GameOverHud->FaderWhite;
	}
}

void UMT_WhiteScreenFade::OnSetTime(AMeteorTimer* MeteorTimer)
{
	IMeteorTimed::OnSetTime(MeteorTimer);
	const UMeteorTimerDataAsset* Settings = MeteorTimer->Settings;
	if (const float ImpactTime = MeteorTimer->GetTimer(); ImpactTime <= Settings->SecondsLeftWhenToFade)
	{
		const float Percent = MeteorTimer->GetTimer() / Settings->SecondsLeftWhenToFade;
		if (Fader)
		{
			//	exponential interpolation
			const auto PercentReverse = 1.f - Percent;
			const float t = FMath::Lerp(0.f, 1.f, Easings::QuartIn(PercentReverse));
			Fader->SetFadeOpacity(t);
		}
	}
	else
	{
		Fader->SetFadeOpacity(0.f);
	}
}
