// Fill out your copyright notice in the Description page of Project Settings.


#include "CutsceneSubsystem.h"

#include "MSGameMode.h"

void UCutsceneSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UCutsceneSubsystem::StartCutscene(TSubclassOf<UUserWidget> CutsceneWidget)
{
	auto GameMode = AMSGameMode::GetGameMode(this);
	if(GameMode)
	{
		GameMode->SetCutsceneMode(this, true);
	}
}
