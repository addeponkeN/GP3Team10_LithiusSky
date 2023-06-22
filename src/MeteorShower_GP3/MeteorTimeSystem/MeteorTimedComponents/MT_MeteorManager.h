#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeteorShower_GP3/DataAssets/MeteorManagerDataAsset.h"
#include "MeteorShower_GP3/MeteorTimeSystem/MeteorTimed.h"
#include "MT_MeteorManager.generated.h"


USTRUCT()
struct FMeteorConfig
{
	GENERATED_BODY()

	FMeteorConfig() : Offset(FVector::ZeroVector)
	{
	}

	explicit FMeteorConfig(const FVector& Offset)
		: Offset(Offset)
	{
	}

	UPROPERTY()
	FVector Offset;

	UPROPERTY()
	FVector DestinationOffset;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class METEORSHOWER_GP3_API UMT_MeteorManager : public UActorComponent, public IMeteorTimed
{
	GENERATED_BODY()

public:
	UMT_MeteorManager();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	UMeteorManagerDataAsset* Settings;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> BigMeteors;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> SmallMeteors;

	UPROPERTY()
	TArray<FMeteorConfig> BigMeteorConfigs;

	UPROPERTY()
	TArray<FMeteorConfig> SmallMeteorConfigs;

	UFUNCTION()
	virtual void OnSetTime(AMeteorTimer* MeteorTimer) override;

	UFUNCTION(BlueprintCallable)
	void SetMeteorShowerProgress(float MeteorProgress);

	void HandleMeteors(TArray<AActor*> MeteorsToHandle, TArray<FMeteorConfig> MeteorConfigs, float MeteorProgress, const FMeteorDataSettings& MeteorSettings);

private:
};
