#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LoopManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoopRestarted);

UCLASS()
class METEORSHOWER_GP3_API ALoopManager : public AActor
{
	GENERATED_BODY()

public:
	ALoopManager();

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnLoopRestarted OnLoopRestarted;

	//	doesnt actually restart the loop.. but it broadcasts the event. NO TIME FOR PROPER IMPL
	UFUNCTION(BlueprintCallable)
	void RestartLoop();

	UFUNCTION()
	void ResetCounter();

	UPROPERTY(BlueprintReadWrite)
	int LoopCount;
	
};
