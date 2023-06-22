#pragma once

#include "CoreMinimal.h"
#include "PoolCollection.generated.h"

class APoolable;

USTRUCT()
struct METEORSHOWER_GP3_API FPoolCollection
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<APoolable*> Items;
};
