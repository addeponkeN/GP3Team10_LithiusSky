// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeteorTimedSoundTriggerDataAsset.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FSoundTriggerNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USoundBase*> Sounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WhenToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool PlayOncePerGameSession;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSFX;

	bool bPlayed;

	USoundBase* GetRandomSound()
	{
		const int TriggerSoundsCount = Sounds.Num();
		if (TriggerSoundsCount <= 1)
			return Sounds[0];
		return Sounds[FMath::RandRange(0, TriggerSoundsCount - 1)];
	}
};

UCLASS()
class METEORSHOWER_GP3_API UMeteorTimedSoundTriggerDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSoundTriggerNode> SoundTriggers;
};
