// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenuUi.h"

#include "Kismet/GameplayStatics.h"
#include "MeteorShower_GP3/MSGameInstance.h"
#include "MeteorShower_GP3/MSGameMode.h"

void UGameMenuUi::NativeConstruct()
{
	Super::NativeConstruct();
	BtResume->OnClicked.AddDynamic(this, &UGameMenuUi::BtResume_Clicked);
	BtRewind->OnClicked.AddDynamic(this, &UGameMenuUi::BtRewind_Clicked);
	BtQuit->OnClicked.AddDynamic(this, &UGameMenuUi::BtQuit_Clicked);
}

void UGameMenuUi::BtResume_Clicked()
{
	auto GameMode = AMSGameMode::GetGameMode(GetWorld());
	GameMode->UnpauseGame();
}

void UGameMenuUi::BtRewind_Clicked()
{
	auto GameInstance = GetWorld()->GetGameInstance<UMSGameInstance>();
	GameInstance->ResetCheckpoints();
	auto LevelName = TEXT("Level_Final");
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
	// GameInstance->LoadGameState();
}

void UGameMenuUi::BtQuit_Clicked()
{
#if !UE_EDITOR
	FPlatformMisc::RequestExit(false);
#endif
}
