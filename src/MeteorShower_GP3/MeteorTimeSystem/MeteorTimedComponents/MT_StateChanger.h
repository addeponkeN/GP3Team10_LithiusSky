#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeteorShower_GP3/MeteorTimeSystem/MeteorTimed.h"
#include "MT_StateChanger.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class METEORSHOWER_GP3_API UMT_StateChanger : public UActorComponent, public IMeteorTimed
{
	GENERATED_BODY()

public:
	UMT_StateChanger();

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnSetTime(AMeteorTimer* MeteorTimer) override;

	UFUNCTION()
	void OnTimerStopped(AMeteorTimer* MeteorTimer);

private:
	bool bRespawned;
};
