#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeteorShower_GP3/MeteorTimeSystem/MeteorTimed.h"
#include "MT_SoundTrigger.generated.h"

class UMeteorTimedSoundTriggerDataAsset;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAudioTriggered);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class METEORSHOWER_GP3_API UMT_SoundTrigger : public UActorComponent, public IMeteorTimed
{
	GENERATED_BODY()

public:
	UMT_SoundTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMeteorTimedSoundTriggerDataAsset* Settings;

protected:
	virtual void Register(AMeteorTimer* MeteorTimer) override;
	virtual void OnSetTime(AMeteorTimer* MeteorTimer) override;

private:
	UFUNCTION()
	void OnTimerReset(AMeteorTimer* MeteorTimer);

};
