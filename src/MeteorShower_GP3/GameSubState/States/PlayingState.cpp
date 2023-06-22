#include "PlayingState.h"

#include "DeadState.h"
#include "MeteorShower_GP3/MSGameMode.h"

APlayingState::APlayingState()
{
	PrimaryActorTick.bCanEverTick = false;
	StateType = ESubGameStates::Playing;
}

void APlayingState::BeginPlay()
{
	Super::BeginPlay();
	Character = AMSGameMode::GetCharacter(GetWorld());
}

void APlayingState::StartEnterState()
{
	Super::StartEnterState();
	Character->HealthComponent->OnKilledEvent.AddDynamic(this, &APlayingState::OnCharacterDied);
}

void APlayingState::OnEnteredState()
{
	Super::OnEnteredState();
	Character->SetInputEnabled(true);
}

void APlayingState::OnExitedState()
{
	Super::OnExitedState();
	Character->HealthComponent->OnKilledEvent.RemoveDynamic(this, &APlayingState::OnCharacterDied);
}

void APlayingState::OnCharacterDied()
{
	AMSGameMode* GameMode = AMSGameMode::GetGameMode(this);
	
	//	dont respawn if meteor has impacted, wait for GIGA respawn
	if (GameMode->GetMeteorTimer()->GetTimer() <= 0.1f)
	{
		LOG("stopped respawn after meteor impact");
	}
	else
	{
		StateManager->SetState<ADeadState>();
	}
}
