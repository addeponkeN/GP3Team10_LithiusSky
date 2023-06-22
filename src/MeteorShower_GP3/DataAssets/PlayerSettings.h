#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerSettings.generated.h"

UCLASS()
class METEORSHOWER_GP3_API UPlayerSettings : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float TimeGunStopDuration;
	
};
