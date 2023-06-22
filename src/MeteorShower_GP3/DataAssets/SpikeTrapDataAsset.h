// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpikeTrapDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API USpikeTrapDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * @brief Time it takes before the spikes emerge after being triggered
	 */
	UPROPERTY(EditAnywhere)
	float TriggerTime{.15f};

	/**
	 * @brief Trigger cooldown
	 */
	UPROPERTY(EditAnywhere)
	float Cooldown{1.f};

	/**
	 * @brief The time the spikes stay out after being triggered
	 */
	UPROPERTY(EditAnywhere)
	float WaitTime{1.5f};

	/**
	 * @brief Time it takes for the spikes to submerge into hiding
	 */
	UPROPERTY(EditAnywhere)
	float SubmergeTime{1.5f};
};
