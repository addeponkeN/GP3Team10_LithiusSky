
#include "LandingState.h"
#include "PlayingState.h"
#include "MeteorShower_GP3/MeteorShower_GP3Character.h"
#include "MeteorShower_GP3/MSGameMode.h"

ALandingState::ALandingState()
{
	PrimaryActorTick.bCanEverTick = false;
	StateType = ESubGameStates::Landing;
	ExitTime = 0.75f;
}

void ALandingState::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALandingState::StartEnterState()
{
	Super::StartEnterState();
	//	set screen to black
	auto GameMode = AMSGameMode::GetGameMode(GetWorld());
	GameMode->GameOverHud->GetFaderBlack()->StartFade(ExitTime + 1.5f, 1.f, 0.f);
	GameMode->GameOverHud->GetFaderBlack()->SetFadeOpacity(1.f);
}

void ALandingState::OnEnteredState()
{
	Super::OnEnteredState();
	StartExitState();
}

void ALandingState::StartExitState()
{
	Super::StartExitState();
}

void ALandingState::OnExitedState()
{
	Super::OnExitedState();
	//	enable input
	auto Controller = GetWorld()->GetFirstPlayerController();
	if(!Controller)
		return;
	auto Character = Cast<AMeteorShower_GP3Character>(Controller->GetCharacter());
	if(!Character)
		return;
	Character->SetInputEnabled(true);
	
	auto GameMode = AMSGameMode::GetGameMode(GetWorld());
	if(!GameMode)
		return;
	
	GameMode->GetMeteorTimer()->StartTimer();
	GameMode->ResetHighScoreTimer();
	GameMode->ResumeHighScoreTimer();
	
	StateManager->SetState<APlayingState>();
}
