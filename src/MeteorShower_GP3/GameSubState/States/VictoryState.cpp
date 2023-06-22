// Fill out your copyright notice in the Description page of Project Settings.


#include "VictoryState.h"

#include "MeteorShower_GP3/MSGameMode.h"

void AVictoryState::BeginPlay()
{
	Super::BeginPlay();
	StateType = ESubGameStates::Victory;
	Character = AMSGameMode::GetCharacter(GetWorld());
	EnterTime = 1.5f;
	ExitTime = 0.f;
}

void AVictoryState::StartEnterState()
{
	Super::StartEnterState();
	auto GameMode = AMSGameMode::GetGameMode(this);
	if (GameMode)
	{
		auto FaderImage = GameMode->GameOverHud->GetFaderBlack();
		if (FaderImage)
		{
			FaderImage->StartFade(EnterTime + 1.5f, 1.f, 0.f);
			FaderImage->SetFadeOpacity(1.f);
		}
	}
	AMSGameMode::SetCharacterInputEnabled(this, false);
}

void AVictoryState::OnEnteredState()
{
	Super::OnEnteredState();
}

void AVictoryState::StartExitState()
{
	Super::StartExitState();
}

void AVictoryState::OnExitedState()
{
	Super::OnExitedState();
}
