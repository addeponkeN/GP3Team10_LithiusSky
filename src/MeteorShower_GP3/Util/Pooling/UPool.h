
#pragma once

#include "CoreMinimal.h"

#include "Poolable.h"
#include "PoolCollection.h"

#include "UPool.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPoolClearedEvent);

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API APool : public AActor
{
	GENERATED_BODY()

public:
	APool();

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UPROPERTY()
	FOnPoolClearedEvent OnClearedEvent;

	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType="ClassTemplate"))
	APoolable* Spawn(UObject* WorldContext, TSubclassOf<AActor> ClassTemplate,
	                 FVector const& Location, FRotator const& Rotation);

	UFUNCTION(BlueprintCallable)
	void Return(APoolable* Actor);

private:
	UPROPERTY(VisibleAnywhere)
	TMap<TSubclassOf<AActor>, FPoolCollection> Pools;
};
