#include "BaseSubState.h"

// #include "MeteorShower_GP3/MeteorShower_GP3.h"

ABaseSubState::ABaseSubState()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseSubState::BeginPlay()
{
	Super::BeginPlay();
	TimerManager = &GetWorldTimerManager();
}

void ABaseSubState::StartEnterState()
{
	bExited = false;
	bExiting = false;
	// LOGF("Starting %d", State);
	if (EnterTime <= 0.f)
	{
		OnEnteredState();
	}
	else
	{
		TimerManager->SetTimer(TimerHandle, this, &ABaseSubState::OnEnteredState, EnterTime);
	}
}

void ABaseSubState::OnEnteredState()
{
	// LOGF("Entered %d", State);
}

void ABaseSubState::StartExitState()
{
	if(bExiting)
	{
		//	already exiting
		return;
	}
	
	bExiting = true;
	// LOGF("Exiting %d", State);
	if (ExitTime <= 0.f)
	{
		OnExitedState();
	}
	else
	{
		TimerManager->SetTimer(TimerHandle, this, &ABaseSubState::OnExitedState, ExitTime);
	}
}

void ABaseSubState::OnExitedState()
{
	bExited = true;
	OnStateExitedEvent.Broadcast();
	// LOGF("Exited %d", State);
}
