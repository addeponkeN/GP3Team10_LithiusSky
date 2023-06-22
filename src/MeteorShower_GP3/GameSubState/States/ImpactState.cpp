// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpactState.h"

#include "PlayingState.h"
#include "MeteorShower_GP3/LoopManager.h"
#include "MeteorShower_GP3/MSGameInstance.h"
#include "MeteorShower_GP3/MSGameMode.h"

AImpactState::AImpactState()
{
	StateType = ESubGameStates::Impact;
	EnterTime = 1.f;
	ExitTime = 2.f;
}

void AImpactState::BeginPlay()
{
	Super::BeginPlay();
	const auto World = GetWorld();
	GameMode = AMSGameMode::GetGameMode(World);
	Character = AMSGameMode::GetCharacter(World);
}

//	the screen should be fully white here (GameOverHud->FaderWhite is at 1 opacity)
void AImpactState::StartEnterState()
{
	Super::StartEnterState();
	APlayerController* PlayerController = Character->GetController<APlayerController>();
	Character->DisableInput(PlayerController);
}

void AImpactState::OnEnteredState()
{
	Super::OnEnteredState();
	Character->CheckpointManager->ClearCurrentCheckpoint();
	UMSGameInstance* Instance = GetWorld()->GetGameInstance<UMSGameInstance>();
	Instance->LoadGameState();
	StartExitState();
	GameMode->GetLoopManager()->RestartLoop();
}

void AImpactState::StartExitState()
{
	Super::StartExitState();
	GameMode->GetMeteorTimer()->StopTimer();
	GameMode->GameOverHud->FaderWhite->SetFadeOpacity(1.f);
	GameMode->GameOverHud->FaderWhite->StartFade(ExitTime, 1.f, 0.f);
}

void AImpactState::OnExitedState()
{
	Super::OnExitedState();
	GameMode->GetMeteorTimer()->StartTimer();
	StateManager->SetState<APlayingState>();
}
