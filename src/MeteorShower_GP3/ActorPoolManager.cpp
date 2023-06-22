#include "ActorPoolManager.h"
#include "Util/Pooling/ActorPool.h"

APool* UActorPoolManager::Pool;

bool UActorPoolManager::bWarnedKillZ;

APool* UActorPoolManager::GetPool()
{
	return Pool;
}

APoolable* UActorPoolManager::Spawn(UObject* WorldContext, TSubclassOf<APoolable> ClassTemplate,
                                    FVector const Position, FRotator const Rotation)
{
	UWorld* world = WorldContext->GetWorld();

	if (!world)
	{
		LOGE("Invalid World in UActorPoolManager:Spawn");
		return nullptr;
	}

	if (!bWarnedKillZ)
	{
		bWarnedKillZ = true;
		const AWorldSettings* Settings = world->GetWorldSettings();
		if (Settings->bEnableWorldBoundsChecks)
		{
			LOGW(
				"POOL_WARNING: 'WorldSettings::bEnableWorldBoundsChecks' is TRUE. Recommended is FALSE. Optionally set 'WorldSettings::KillZ' to an extremely low value.");
		}
	}

	if (bPoolIsDead || !Pool || !IsValid(Pool))
	{
		Pool = world->SpawnActor<APool>(APool::StaticClass());
		bPoolIsDead = false;
		LOGI("Created a pool");
	}

	return GetPool()->Spawn(WorldContext, ClassTemplate, Position, Rotation);
}

void UActorPoolManager::Return(APoolable* Actor)
{
	if (!Actor)
	{
		LOGE("Actor was null on UActorPoolManager::Return");
		return;
	}
	GetPool()->Return(Actor);
}

void UActorPoolManager::ResetPool()
{
	bPoolIsDead = true;
	bWarnedKillZ = false;
}
