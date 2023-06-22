#include "GameSubStateManager.h"

#include "MeteorShower_GP3/MeteorShower_GP3.h"

AGameSubStateManager::AGameSubStateManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGameSubStateManager::BeginPlay()
{
	Super::BeginPlay();
}

void AGameSubStateManager::SetState(ABaseSubState* State)
{
	if (NextState)
	{
		LOGW("A state is already queued");
		return;
	}

	State->StateManager = this;

	if (CurrentState)
	{
		if (CurrentState->bExited)
		{
			CurrentState = State;
			CurrentState->StartEnterState();
			LOGF("Set State: %s", *CurrentState->GetName());
		}
		else
		{
			NextState = State;
			ExitCurrent();
		}
	}
	else
	{
		CurrentState = State;
		CurrentState->StartEnterState();
		OnStateChanged.Broadcast(CurrentState->StateType);
		LOGF("Set State: %s", *CurrentState->GetName());
	}
}

void AGameSubStateManager::ExitCurrent()
{
	// LOG("waiting for state to exit");
	CurrentState->OnStateExitedEvent.AddDynamic(this, &AGameSubStateManager::CurrentStateExited);
	CurrentState->StartExitState();
}

void AGameSubStateManager::CurrentStateExited()
{
	// LOG("waiting for state to exit - DONE");
	CurrentState->OnStateExitedEvent.RemoveDynamic(this, &AGameSubStateManager::CurrentStateExited);
	CurrentState = NextState;
	NextState = nullptr;
	CurrentState->StartEnterState();
	LOGF("Set State: %s", *CurrentState->GetName());
}
