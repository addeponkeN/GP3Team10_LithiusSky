#include "MT_SoundTrigger.h"

#include "MeteorShower_GP3/MeteorShower_GP3.h"
#include "MeteorShower_GP3/SoundHelper.h"
#include "MeteorShower_GP3/DataAssets/MeteorTimedSoundTriggerDataAsset.h"

UMT_SoundTrigger::UMT_SoundTrigger()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMT_SoundTrigger::Register(AMeteorTimer* MeteorTimer)
{
	IMeteorTimed::Register(MeteorTimer);
	LOG("soundtrigger register");
	MeteorTimer->OnTimerStoppedEvent.AddDynamic(this, &UMT_SoundTrigger::OnTimerReset);
	for (int i = 0; i < Settings->SoundTriggers.Num(); ++i)
	{
		Settings->SoundTriggers[i].bPlayed = false;
	}
}

void UMT_SoundTrigger::OnSetTime(AMeteorTimer* MeteorTimer)
{
	IMeteorTimed::OnSetTime(MeteorTimer);

	const float ImpactTimer = MeteorTimer->GetTimer();
	auto& Triggers = Settings->SoundTriggers;

	for (int i = 0; i < Triggers.Num(); ++i)
	{
		FSoundTriggerNode& Trigger = Triggers[i];

		if (Trigger.Sounds.Num() <= 0)
			continue;

		if (Trigger.bPlayed)
		{
			if (ImpactTimer > Trigger.WhenToPlay)
			{
				Trigger.bPlayed = false;
			}
			continue;
		}

		if (ImpactTimer <= Trigger.WhenToPlay)
		{
			USoundBase* Sound = Trigger.GetRandomSound();

			if (Sound)
			{
				if (Trigger.bSFX)
				{
					USoundHelper::PlaySoundSFX(this, Sound);
				}
				else
				{
					USoundHelper::PlayDialogueSound(this, Sound);
				}
				LOG("Played sound");
				Trigger.bPlayed = true;
			}
		}
	}
}


void UMT_SoundTrigger::OnTimerReset(AMeteorTimer* MeteorTimer)
{
	LOG("TimeReset soundtrigger");
	auto& Triggers = Settings->SoundTriggers;
	for (int i = 0; i < Triggers.Num(); ++i)
	{
		FSoundTriggerNode& Trigger = Triggers[i];
		Trigger.bPlayed = false;
	}
	// bPlayed = false;
}
