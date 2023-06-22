#include "MeteorTimer.h"

#include "MeteorShower_GP3/MSGameMode.h"
#include "MeteorTimedComponents/MT_SoundTrigger.h"

AMeteorTimer::AMeteorTimer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMeteorTimer::BeginPlay()
{
	Super::BeginPlay();

	auto Character = AMSGameMode::GetCharacter(GetWorld());
	if (!Character)
	{
		return;
	}

	Timer = MeteorImpactTime;
	auto MeteorTimedComponents = GetComponentsByInterface(UMeteorTimed::StaticClass());
	for (UActorComponent* MeteorTimedActor : MeteorTimedComponents)
	{
		if (IMeteorTimed* MeteorTimedInterface = Cast<IMeteorTimed>(MeteorTimedActor))
		{
			MeteorTimedInterface->Register(this);
		}
	}
	OnTimerChangedEvent.Broadcast(this);
}

// void AMeteorTimer::AddSoundTrigger(USoundBase* Sound, float TimeLeftWhenToPlay, bool PlayOncePerGameSession)
// {
// 	UMT_SoundTrigger* AudioTrigger = NewObject<UMT_SoundTrigger>();
// 	AudioTrigger->BindAudio(Sound, TimeLeftWhenToPlay, PlayOncePerGameSession);
// 	
// 	if (IMeteorTimed* const AudioTriggerMeteorTimedInterface = Cast<IMeteorTimed>(AudioTrigger))
// 	{
// 		AudioTriggerMeteorTimedInterface->Register(this);
// 	}
// 	
// 	AddOwnedComponent(AudioTrigger);
// }

void AMeteorTimer::SetTimerEnabled(bool EnableTimer)
{
	bTimerEnabled = EnableTimer;
	if (bTimerEnabled)
	{
		StartTimer();
	}
	else
	{
		StopTimer();
	}
}

void AMeteorTimer::SetTimer(float Value)
{
	Timer = FMath::Clamp(Value, 0.f, MeteorImpactTime);
	auto Character = AMSGameMode::GetCharacter(GetWorld());
	if (!Character)
	{
		return;
	}
	OnTimerChangedEvent.Broadcast(this);
}

void AMeteorTimer::StartTimer()
{
	auto Character = AMSGameMode::GetCharacter(GetWorld());
	if (!Character)
	{
		return;
	}

	if (bTimerEnabled)
		return;

	bTimerEnabled = true;
	GetWorldTimerManager().SetTimer(IncrementTimerHandle, this, &AMeteorTimer::DecreaseTimer, TimerTickRate, true);

	if (Timer >= MeteorImpactTime - 0.1f)
	{
		OnTimerStartedEvent.Broadcast(this);
	}
}

void AMeteorTimer::StopTimer()
{
	if (!bTimerEnabled)
		return;

	SetTimer(MeteorImpactTime);
	// Timer = MeteorImpactTime;
	bTimerEnabled = false;
	GetWorldTimerManager().ClearTimer(IncrementTimerHandle);
	OnTimerStoppedEvent.Broadcast(this);
}

void AMeteorTimer::PauseTimer()
{
	if (!bTimerEnabled)
		return;

	bTimerEnabled = false;
	GetWorldTimerManager().ClearTimer(IncrementTimerHandle);
}

FString AMeteorTimer::GetFormattedString()
{
	const float Time = GetTimer();
	const int32 Minutes = static_cast<int32>(Time) / 60;
	const float Seconds = static_cast<int32>(Time) % 60;
	const float Milliseconds = (Time - FMath::FloorToInt(Time)) * 100;

	FString FormattedTime;
	if (Minutes > 0)
	{
		FormattedTime = FString::Printf(TEXT("%d:%02.0f.%02.0f"), Minutes, Seconds, Milliseconds);
	}
	else if (Seconds < 10)
	{
		FormattedTime = FString::Printf(TEXT("%.0f.%02.0f"), Seconds, Milliseconds);
	}
	else
	{
		FormattedTime = FString::Printf(TEXT("%02.0f.%02.0f"), Seconds, Milliseconds);
	}

	return FormattedTime;
}

void AMeteorTimer::DecreaseTimer()
{
	SetTimer(Timer - TimerTickRate);
	// const int PrintTime = Timer;
	// LOGF("Meteor impact in: %i", PrintTime);
}
