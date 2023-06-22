// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeteorManagerDataAsset.generated.h"

USTRUCT()
struct FMeteorDataSettings
{
	GENERATED_BODY()

	FMeteorDataSettings() = default;

	FMeteorDataSettings(float Scale, const FVector& MeteorRandomDeviation)
		: Scale(Scale),
		  MeteorRandomDeviation(MeteorRandomDeviation)
	{
	}

	UPROPERTY(EditAnywhere, Category=Transform)
	float Scale{30.f};

	UPROPERTY(EditAnywhere, Category=Locations)
	FVector MeteorRandomDeviation{FVector(50000.f, 50000.f, 1500.f)};
	
};

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API UMeteorManagerDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> MeteorClass;

	UPROPERTY(EditAnywhere, Category=Stats)
	float TimeLeftWhenToMoveMeteors{180.f};

	UPROPERTY(EditAnywhere, Category=Stats)
	int BigMeteorCount{6};

	UPROPERTY(EditAnywhere, Category=Stats)
	int SmallMeteorCount{50};

	UPROPERTY(EditAnywhere, Category=Locations)
	float MeteorStartZ{500000.f};

	UPROPERTY(EditAnywhere, Category=Locations)
	FVector2f MeteorXYOffset{FVector2f(400000.f, 200000.f)};

	UPROPERTY(EditAnywhere, Category=Locations)
	FVector MeteorDestination{FVector::ZeroVector};
	
	UPROPERTY(EditAnywhere)
	FMeteorDataSettings BigMeteors{FMeteorDataSettings(100.f, FVector(50000.f, 50000.f, 1500.f))};

	UPROPERTY(EditAnywhere)
	FMeteorDataSettings SmallMeteors{FMeteorDataSettings(30.f, FVector(500000.f, 500000.f, 30000.f))};
};
