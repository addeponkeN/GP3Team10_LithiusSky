// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHud.h"

#include "EnhancedInputComponent.h"
#include "MeteorShower_GP3/MSGameMode.h"
#include "MeteorShower_GP3/DataAssets/CheckpointManagerDataAsset.h"

void UGameHud::NativeConstruct()
{
	Super::NativeConstruct();

	// AMSGameMode* GameMode = Cast<AMSGameMode>(GetWorld()->GetAuthGameMode());
	// if (GameMode)
	// {
	// 	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	// 	AMeteorShower_GP3Character* Character = AMSGameMode::GetCharacter(GetWorld());
	// 	UCheckpointManager* CPManager = Character->CheckpointManager;
	// 	UEnhancedInputComponent* CharacterInputComponent = Cast<UEnhancedInputComponent>(Controller->InputComponent);
	// 	CharacterInputComponent->BindAction(
	// 		CPManager->Settings->RestartAction, ETriggerEvent::Triggered, this, &UGameHud::OnRestartAction);
	// }
}

void UGameHud::OnRestartAction()
{
	LOG("UI RESTART GO");
}
