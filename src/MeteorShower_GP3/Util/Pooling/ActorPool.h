// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Poolable.h"
#include "MeteorShower_GP3/MeteorShower_GP3.h"

//////////////////////////////////////////////////
////////////////////HEADER////////////////////////
//////////////////////////////////////////////////

template <class T>
class ActorPool
{
	static_assert(std::is_base_of_v<APoolable, T>, "T must be derived APoolable");

public:
	static void Clear();
	static T* Create(UObject const* worldContext, TSubclassOf<AActor> objectTemplate,
	                 FVector const* position, FRotator const* rotation);
	static T* Spawn(UObject const* worldContext, TSubclassOf<AActor> objectTemplate);
	static T* Spawn(UObject const* worldContext, TSubclassOf<AActor> objectTemplate, FVector const* position);
	static T* Spawn(UObject const* worldContext, TSubclassOf<AActor> objectTemplate, FVector const* position,
	                FRotator const* rotation);
	static void Return(T* obj);

private:
	static UWorld* _prevWorld;
	static FName _name;
	static TMap<TSubclassOf<AActor>, TArray<T*>> _mapPool;
};

//////////////////////////////////////////////////
//////////////////////CPP/////////////////////////
//////////////////////////////////////////////////

template <typename T>
UWorld* ActorPool<T>::_prevWorld;

template <typename T>
FName ActorPool<T>::_name;

template <typename T>
TMap<TSubclassOf<AActor>, TArray<T*>> ActorPool<T>::_mapPool;

template <typename T>
void ActorPool<T>::Clear()
{
	_mapPool.Empty();
}

template <typename T>
T* ActorPool<T>::Create(UObject const* worldContext, TSubclassOf<AActor> objectTemplate,
                        FVector const* position, FRotator const* rotation)
{
	T* actor = worldContext->GetWorld()->SpawnActor<T>(objectTemplate, *position, *rotation);
	if (!actor)
	{
		LOG("failed to spawn actor T");
		return nullptr;
	}
	actor->OnCreated(_name);
	return actor;
}

template <typename T>
T* ActorPool<T>::Spawn(UObject const* worldContext, TSubclassOf<AActor> objectTemplate)
{
	return Spawn(worldContext, objectTemplate, &FVector::ZeroVector, &FRotator::ZeroRotator);
}

template <typename T>
T* ActorPool<T>::Spawn(UObject const* worldContext, TSubclassOf<AActor> objectTemplate, FVector const* position)
{
	return Spawn(worldContext, objectTemplate, position, &FRotator::ZeroRotator);
}

template <typename T>
T* ActorPool<T>::Spawn(UObject const* worldContext, TSubclassOf<AActor> objectTemplate, FVector const* position,
                       FRotator const* rotation)
{
	if (!worldContext)
	{
		LOG("WorldContext was null");
		return nullptr;
	}

	if (!objectTemplate)
	{
		LOG("Template null");
		return nullptr;
	}

	if (_prevWorld != worldContext->GetWorld())
	{
		Clear();
		LOG(FString::Printf(TEXT("Cleared pool '%s'"), *objectTemplate->GetName()));
	}

	_prevWorld = worldContext->GetWorld();

	TArray<T*>* arr;

	if (!_mapPool.Contains(objectTemplate))
	{
		arr = &_mapPool.Add(objectTemplate);
	}
	else
	{
		arr = &_mapPool[objectTemplate];
	}

	T* ret;

	if (arr->IsEmpty())
	{
		ret = Create(worldContext, objectTemplate, position, rotation);
		if (!ret)
		{
			LOG("failed to create actor");
			return nullptr;
		}
	}
	else
	{
		ret = arr->Pop(false);
		// if (!IsValid(ret))
		// {
		// LOG("outactor not valid!!");
		// ret = Create(worldContext, objectTemplate, position, rotation);
		// }
	}

	ret->SetActorLocation(*position);
	ret->SetActorRotation(*rotation);
	ret->Spawned();

	return ret;
}

template <typename T>
void ActorPool<T>::Return(T* obj)
{
	obj->Reset();
	obj->Returned();
	auto classType = obj->GetClass();
	_mapPool[classType].Add(obj);
}
