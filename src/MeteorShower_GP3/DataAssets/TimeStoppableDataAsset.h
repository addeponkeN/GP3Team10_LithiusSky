#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TimeStoppableDataAsset.generated.h"

UCLASS()
class METEORSHOWER_GP3_API UTimeStoppableDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* OnStoppedSound;

	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* OnResumedSound;
	
};
