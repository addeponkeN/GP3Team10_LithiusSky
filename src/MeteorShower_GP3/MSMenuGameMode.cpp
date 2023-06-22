
#include "MSMenuGameMode.h"

#include "MenuManager.h"
#include "DataAssets/MenuCollectionDataAsset.h"

void AMSMenuGameMode::StartPlay()
{
	Super::StartPlay();
}

void AMSMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	MenuManager = NewObject<UMenuManager>(this, FName("MainMenuManager"));
	SetMenu(MenuCollection->StartMenu);
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	Controller->SetInputMode(FInputModeUIOnly());
	Controller->SetShowMouseCursor(true);
}

void AMSMenuGameMode::SetMenu(TSubclassOf<UMenuBase> MenuClass)
{
	MenuManager->SetMenu(MenuClass);
}
