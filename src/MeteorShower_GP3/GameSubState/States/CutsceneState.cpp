
#include "CutsceneState.h"

#include "MeteorShower_GP3/MSGameMode.h"

ACutsceneState::ACutsceneState()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACutsceneState::BeginPlay()
{
	Super::BeginPlay();
	GameMode = AMSGameMode::GetGameMode(GetWorld());
}

void ACutsceneState::StartEnterState()
{
	Super::StartEnterState();
	GameMode->SetShowCharacterGameUi(false);
	GameMode->SetCharacterInputEnabled_CPP(false);
}

void ACutsceneState::OnEnteredState()
{
	Super::OnEnteredState();
	
}

void ACutsceneState::StartExitState()
{
	Super::StartExitState();
	
}

void ACutsceneState::OnExitedState()
{
	Super::OnExitedState();
	GameMode->SetShowCharacterGameUi(true);
	GameMode->SetCharacterInputEnabled_CPP(true);
}
