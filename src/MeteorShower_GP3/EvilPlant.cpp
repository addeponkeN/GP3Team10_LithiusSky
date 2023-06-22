#include "EvilPlant.h"

#include "ActorPoolManager.h"
#include "MeteorShower_GP3.h"
#include "TimeStoppable.h"

AEvilPlant::AEvilPlant()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEvilPlant::BeginPlay()
{
	Super::BeginPlay();
	Mesh = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	GetWorldTimerManager().SetTimer(handle, this, &AEvilPlant::Push, FMath::FRandRange(2.f, 3.f), true);
	// GetWorldTimerManager().SetTimer(spawnTimerHandle, this, &AEvilPlant::SpawnPlant, FMath::FRandRange(1.f, 1.01f));
}

void AEvilPlant::Push()
{
	if (Mesh)
	{
		if (UTimeStoppable* TimeStoppable = Cast<UTimeStoppable>(GetComponentByClass(UTimeStoppable::StaticClass())))
		{
			if (TimeStoppable->bFrozen)
				return;
		}
		int32 power = FMath::RandRange(50000, 150000);
		Mesh->AddImpulse(FVector(0, 0, power));
	}
}

void AEvilPlant::SpawnPlant()
{
	auto location = GetActorLocation() + FVector(0, 0, 100);
	auto finalTemplate = FMath::RandBool() ? ClassTemplate : ClassTemplate2;
	spawned = UActorPoolManager::Spawn(this, finalTemplate, location, FRotator::ZeroRotator);
	GetWorldTimerManager().SetTimer(spawnTimerHandle, this, &AEvilPlant::KillPlant, FMath::FRandRange(0.95f, 1.f));
}

void AEvilPlant::KillPlant()
{
	if (!spawned || !IsValid(spawned))
	{
		LOG("spawned object invalid on KillPlant");
	}
	else
	{
		spawned->Kill();
	}
	GetWorldTimerManager().SetTimer(spawnTimerHandle, this, &AEvilPlant::SpawnPlant, FMath::FRandRange(1.f, 1.05f));
}
