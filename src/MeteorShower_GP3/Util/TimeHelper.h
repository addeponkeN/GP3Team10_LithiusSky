#pragma once

#include "CoreMinimal.h"

#include "TimeHelper.generated.h"

UCLASS()
class METEORSHOWER_GP3_API UTimeHelper : public UObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	static FString FormatToTime(float Time);

	UFUNCTION(BlueprintCallable)
	static FString FormatToTimeFull(float Time);
};
