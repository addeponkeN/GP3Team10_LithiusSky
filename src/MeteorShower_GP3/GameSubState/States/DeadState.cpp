#include "DeadState.h"

#include "PlayingState.h"

ADeadState::ADeadState()
{
	StateType = ESubGameStates::Dead;
	EnterTime = 0.5f;
	ExitTime = 0.25f;
}

void ADeadState::BeginPlay()
{
	Super::BeginPlay();
	GameMode = AMSGameMode::GetGameMode(GetWorld());
	Character = AMSGameMode::GetCharacter(GetWorld());
}

void ADeadState::StartEnterState()
{
	Super::StartEnterState();
	
	//	make sure screen fades to black before entered
	const float FadeToBlackTime = EnterTime - 0.075f;
	GameMode->GameOverHud->GetFaderBlack()->StartFade(FadeToBlackTime, 0.f, 1.f);
	
	Character->SetInputEnabled(false);
}

void ADeadState::OnEnteredState()
{
	Super::OnEnteredState();
	
	//	respawn player
	Character->CheckpointManager->RespawnAtCheckpoint();
	
	//	enter playing state
	StateManager->SetState<APlayingState>();
}

void ADeadState::StartExitState()
{
	Super::StartExitState();

	const float FadeToWhiteTime = EnterTime + 0.5f;
	GameMode->GameOverHud->GetFaderBlack()->StartFade(FadeToWhiteTime, 1.f, 0.f);
}

void ADeadState::OnExitedState()
{
	Super::OnExitedState();

	Character->SetInputEnabled(true);
}
