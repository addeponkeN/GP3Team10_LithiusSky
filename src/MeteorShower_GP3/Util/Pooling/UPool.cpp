// Fill out your copyright notice in the Description page of Project Settings.


#include "UPool.h"

#include "MeteorShower_GP3/ActorPoolManager.h"
#include "MeteorShower_GP3/MeteorShower_GP3.h"


APool::APool()
{
}

APoolable* APool::Spawn(UObject* WorldContext, TSubclassOf<AActor> ClassTemplate,
                        FVector const& Location, FRotator const& Rotation)
{
	if (!ClassTemplate)
	{
		LOG("class template null!!");
		return nullptr;
	}

	UWorld* World = WorldContext->GetWorld();

	if (!World)
	{
		LOG("world null!!");
		return nullptr;
	}

	APoolable* ret;

	TArray<APoolable*>* arr;

	if (!Pools.Contains(ClassTemplate))
	{
		arr = &Pools.Add(ClassTemplate).Items;
	}
	else
	{
		arr = &Pools[ClassTemplate].Items;
	}

	if (arr->IsEmpty())
	{
		ret = World->SpawnActor<APoolable>(ClassTemplate, Location, Rotation);
	}
	else
	{
		ret = arr->Pop(false);
		if (!ret || !IsValid(ret))
		{
			LOGEF("Pooled actor of type '%s' was null or invalid", *ClassTemplate->GetName());
			ret = World->SpawnActor<APoolable>(ClassTemplate, Location, Rotation);
		}
		
		ret->SetActorLocation(Location);
		ret->SetActorRotation(Rotation);
	}

	ret->Spawned();

	return ret;
}

void APool::Return(APoolable* Actor)
{
	Actor->Reset();
	Actor->Returned();
	auto actorClass = Actor->GetClass();
	Pools[actorClass].Items.Add(Actor);
	// LOGF("returned %s", *actorClass->GetName());
}

void APool::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	LOG("Emptied pool");
	for (auto Element : Pools)
	{
		auto arr = Element.Value;
		arr.Items.Empty();
	}
	Pools.Empty();
	UActorPoolManager::ResetPool();
	Super::EndPlay(EndPlayReason);
}
