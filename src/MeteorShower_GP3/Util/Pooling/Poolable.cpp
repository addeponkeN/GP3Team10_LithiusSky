#include "Poolable.h"

#include "MeteorShower_GP3/ActorPoolManager.h"


APoolable::APoolable()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APoolable::Spawned()
{
	SetActive(true);
}

void APoolable::Returned()
{
	SetActive(false);
}

void APoolable::Kill()
{
	UActorPoolManager::Return(this);
}

void APoolable::OnCreated(const FName& folderPath)
{
	// #ifdef !UE_BUILD_SHIPPING
	// SetFolderPath(folderPath);
	// #endif
}

void APoolable::SetActive(bool isActive)
{
	bIsActive = isActive;
	SetActorTickEnabled(bIsActive);
	SetActorEnableCollision(bIsActive);
	SetActorHiddenInGame(!bIsActive);
}
