
#pragma once

#include "CoreMinimal.h"
#include "ActorSaveState.generated.h"

USTRUCT()
struct METEORSHOWER_GP3_API FActorSaveState
{
	GENERATED_BODY()

	/**
	 * @brief The world-name of the actor
	 */
	UPROPERTY(VisibleAnywhere)
	FString ActorName;
	
	/**
	 * @brief The transform of the saved actor
	 */
	UPROPERTY(VisibleAnywhere)
	FTransform Transform;

	/**
	 * @brief State of the actor (ex: a door which is open(1) or closed(0))
	 */
	UPROPERTY(VisibleAnywhere)
	int8 State;
};
