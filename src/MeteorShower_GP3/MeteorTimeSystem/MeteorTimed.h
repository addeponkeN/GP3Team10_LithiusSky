#pragma once

#include "CoreMinimal.h"
#include "MeteorTimer.h"
#include "UObject/Interface.h"
#include "MeteorTimed.generated.h"

UINTERFACE(MinimalAPI)
class UMeteorTimed : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class METEORSHOWER_GP3_API IMeteorTimed
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnSetTime(AMeteorTimer* MeteorTimer) { }

	UFUNCTION()
	virtual void Register(AMeteorTimer* MeteorTimer);
};
