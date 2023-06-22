#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeteorShower_GP3/MeteorTimeSystem/MeteorTimed.h"
#include "MT_MeteorShards.generated.h"


class UMeteorShardsDataSettings;
class AMeteorShard;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class METEORSHOWER_GP3_API UMT_MeteorShards : public UActorComponent, public IMeteorTimed
{
	GENERATED_BODY()

public:
	UMT_MeteorShards();

protected:
	virtual void BeginPlay() override;
	virtual void Register(AMeteorTimer* MeteorTimer) override;
	virtual void OnSetTime(AMeteorTimer* MeteorTimer) override;


public:
	UFUNCTION(BlueprintCallable)
	void ClearAllShards(AMeteorTimer* MeteorTimer);
	
	UPROPERTY(EditAnywhere)
	UMeteorShardsDataSettings* Settings;
	
	float IntervalTimer;

	UPROPERTY()
	UWorld* World;

	UPROPERTY()
	AActor* CharacterActor;
	
	FTimerHandle SpawnerTimerHandle;

private:
	
	UPROPERTY()
	TArray<AActor*> Shards;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMeteorShard> MeteorShardClass;
	
	UFUNCTION()
	void OnShardDestroyed(AActor* DestroyedShard);
	
	void SpawnMeteor();
	FVector GetRandomLocation();
};




