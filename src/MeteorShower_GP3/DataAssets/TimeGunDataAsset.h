#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TimeGunDataAsset.generated.h"

class ATimeGunLaser;
class UInputAction;
class UInputMappingContext;

UENUM(BlueprintType)
enum class ETimeGunCategory : uint8
{
	Input UMETA(DisplayName = "Input"),
	Stats UMETA(DisplayName = "Stats"),
	SFX UMETA(DisplayName = "SFX"),
};

USTRUCT(BlueprintType)
struct FTimeGunStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlowDuration{6.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlowAmount{0.05f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootRange{2000.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LineTraceIntervalSeconds{0.03223f};
};

USTRUCT(BlueprintType)
struct FTimeGunSFX
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* ShootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* ShootHitTargetSound;
};

UCLASS()
class METEORSHOWER_GP3_API UTimeGunDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* FireTimeGunMappingContext;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* FireTimeGunAction;

	UPROPERTY(EditAnywhere, Category=Collision)
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_GameTraceChannel3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	FTimeGunStats Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SFX")
	FTimeGunSFX SFX;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
	TSubclassOf<ATimeGunLaser> BeamEffectClass;

};
