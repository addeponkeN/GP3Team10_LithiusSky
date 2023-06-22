#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"
#include "MeteorShardsDataSettings.generated.h"

UCLASS()
class METEORSHOWER_GP3_API UMeteorShardsDataSettings : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	float WhenToStart{40.f};

	UPROPERTY(EditAnywhere)
	float SpawnIntervalStart{2.f};
	
	UPROPERTY(EditAnywhere)
	float SpawnIntervalEnd{0.5f};

	UPROPERTY(EditAnywhere)
	float ForwardDistance{3000.f};

	UPROPERTY(EditAnywhere)
	float HeightMin{15000.f};
	
	UPROPERTY(EditAnywhere)
	float HeightMax{30000.f};

	UPROPERTY(EditAnywhere)
	FVector OffsetRange{FVector(2000.f, 2000.f, 0.f)};

	UPROPERTY(EditAnywhere)
	FVector StartOffset{FVector(-1000.f, 3500.f, 0.f)};
	
};
