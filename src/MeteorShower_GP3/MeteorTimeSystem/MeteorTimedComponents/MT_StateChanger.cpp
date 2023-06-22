#include "MT_StateChanger.h"

#include "MeteorShower_GP3/MSGameMode.h"
#include "MeteorShower_GP3/GameSubState/States/ImpactState.h"

UMT_StateChanger::UMT_StateChanger()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMT_StateChanger::BeginPlay()
{
	Super::BeginPlay();
	AMeteorTimer* MeteorTimer = AMSGameMode::GetGameMode(this)->GetMeteorTimer();
	MeteorTimer->OnTimerStoppedEvent.AddDynamic(this, &UMT_StateChanger::OnTimerStopped);
}

void UMT_StateChanger::OnSetTime(AMeteorTimer* MeteorTimer)
{
	IMeteorTimed::OnSetTime(MeteorTimer);

	if (bRespawned)
	{
		return;
	}

	if (MeteorTimer->GetTimer() <= 0.01f)
	{
		bRespawned = true;
		//	respawn at GIGA checkpoint

		AMSGameMode* GameMode = AMSGameMode::GetGameMode(this);
		GameMode->StateManager->SetState<AImpactState>();
	}
}

void UMT_StateChanger::OnTimerStopped(AMeteorTimer* MeteorTimer)
{
	bRespawned = false;
}
