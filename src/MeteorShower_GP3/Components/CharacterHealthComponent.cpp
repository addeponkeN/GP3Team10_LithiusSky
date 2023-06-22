#include "CharacterHealthComponent.h"

#include "MeteorShower_GP3/MeteorShower_GP3.h"

UCharacterHealthComponent::UCharacterHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager()
	          .SetTimer(BoundsTimerHandle, this, &UCharacterHealthComponent::CheckCharacterBounds, 1.f, true);
}

void UCharacterHealthComponent::Kill()
{
	OnKilledEvent.Broadcast();
}

void UCharacterHealthComponent::CheckCharacterBounds()
{
	if (const AActor* Owner = GetOwner())
	{
		if (Owner->GetActorLocation().Z < KillZ)
		{
			Kill();
			LOGW("Fell out of bounds");
		}
	}
}
