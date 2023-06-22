#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Util/Pooling/UPool.h"
#include "ActorPoolManager.generated.h"

UCLASS()
class METEORSHOWER_GP3_API UActorPoolManager : public UObject
{
	GENERATED_BODY()

	inline static bool bPoolIsDead = true;
	static APool* Pool;
	static APool* GetPool();

public:	
	UFUNCTION(BlueprintCallable, Category="Pooling", meta=(DeterminesOutputType="classTemplate"))
	static APoolable* Spawn(UObject* WorldContext, TSubclassOf<APoolable> ClassTemplate,
		FVector const Position = FVector(0,0,0),
		FRotator const Rotation = FRotator(0,0,0));

	UFUNCTION(BlueprintCallable, Category="Pooling")
	static void Return(APoolable* Actor);

	UFUNCTION()
	static void ResetPool();

private:
	static bool bWarnedKillZ;
};
