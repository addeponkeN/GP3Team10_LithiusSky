// Fill out your copyright notice in the Description page of Project Settings.


#include "RestartState.h"

#include "PlayingState.h"
#include "MeteorShower_GP3/LoopManager.h"
#include "MeteorShower_GP3/MSGameInstance.h"
#include "MeteorShower_GP3/MSGameMode.h"

ARestartState::ARestartState()
{
	StateType = ESubGameStates::Restart;
	EnterTime = 0.5f;
	ExitTime = 0.5f;
}

void ARestartState::BeginPlay()
{
	Super::BeginPlay();
	GameMode = AMSGameMode::GetGameMode(GetWorld());
	Character = AMSGameMode::GetCharacter(GetWorld());
}

void ARestartState::StartEnterState()
{
	Super::StartEnterState();

	const float FadeToBlackTime = EnterTime - 0.075f;
	GameMode->GameOverHud->GetFaderBlack()->StartFade(FadeToBlackTime, 0.f, 1.f);
	Character->SetInputEnabled(false);
	GameMode->GetLoopManager()->RestartLoop();
}

void ARestartState::OnEnteredState()
{
	Super::OnEnteredState();

	//	respawn player
	if (bGigaRestart)
	{
		UMSGameInstance* Instance = GetWorld()->GetGameInstance<UMSGameInstance>();
		Instance->LoadGameState();
		Character->CheckpointManager->ClearCurrentCheckpoint();
		GameMode->GetMeteorTimer()->StopTimer();
	}
	else
	{
		Character->CheckpointManager->RespawnAtCheckpoint();
	}

	//	enter playing state
	StateManager->SetState<APlayingState>();
}

void ARestartState::StartExitState()
{
	Super::StartExitState();

	const float FadeToWhiteTime = EnterTime + 0.5f;
	GameMode->GameOverHud->GetFaderBlack()->StartFade(FadeToWhiteTime, 1.f, 0.f);
}

void ARestartState::OnExitedState()
{
	Super::OnExitedState();

	GameMode->GetMeteorTimer()->StartTimer();
	Character->SetInputEnabled(true);
}

void ARestartState::SetIsGigaRestart(bool bIsGigaRestart)
{
	bGigaRestart = bIsGigaRestart;
}
