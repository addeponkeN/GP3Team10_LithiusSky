
#include "SaveableComponent.h"

#include "MeteorShower_GP3/MSGameInstance.h"

USaveableComponent::USaveableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USaveableComponent::BeginPlay()
{
	Super::BeginPlay();
	RegisterSelf();
}

void USaveableComponent::Save()
{
	
}

void USaveableComponent::RegisterSelf()
{
	UMSGameInstance* GameInstance = Cast<UMSGameInstance>(GetWorld()->GetGameInstance());
	if(GameInstance)
	{
		GameInstance->AddSaveable_Implementation(this);
	}
}

