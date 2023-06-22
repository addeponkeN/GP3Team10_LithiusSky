#include "DamageZone.h"

#include "MeteorShower_GP3/MeteorShower_GP3Character.h"
#include "MeteorShower_GP3/Components/CharacterHealthComponent.h"

ADamageZone::ADamageZone()
{
	PrimaryActorTick.bCanEverTick = false;
	Area = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Area"));
	RootComponent = Area;
}

void ADamageZone::BeginPlay()
{
	Super::BeginPlay();
	Area->OnComponentBeginOverlap.AddDynamic(this, &ADamageZone::OnOverlappedTarget);
}

void ADamageZone::OnOverlappedTarget(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	//	temporary 'is character' check
	if (AMeteorShower_GP3Character* Character = Cast<AMeteorShower_GP3Character>(OtherActor))
	{
		
		// auto CheckpointManager = Cast<UCheckpointManager>(
			// Character->GetComponentByClass(UCheckpointManager::StaticClass()));
		// CheckpointManager->RespawnAtCheckpoint();
		
		auto HealthComponent = Cast<UCharacterHealthComponent>(
			Character->GetComponentByClass(UCharacterHealthComponent::StaticClass()));
		HealthComponent->Kill();
		
		// LOG("hit by damagezone");
	}
	else
	{
		// LOG("damage zone hit non-character");
	}
}
