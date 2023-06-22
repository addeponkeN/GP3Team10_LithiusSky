#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeteorShower_GP3/MeteorTimeSystem/MeteorTimed.h"
#include "MT_UIHourGlass.generated.h"

class UImage;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class METEORSHOWER_GP3_API UMT_UIHourGlass : public UActorComponent, public IMeteorTimed
{
	GENERATED_BODY()

public:
	UMT_UIHourGlass();

protected:
	virtual void BeginPlay() override;
	virtual void Register(AMeteorTimer* MeteorTimer) override;
	virtual void OnSetTime(AMeteorTimer* MeteorTimer) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HourglassRotateInterval{5.f};

	UPROPERTY(EditAnywhere)
	float HourglassRotateTime{2.f};


private:
	UPROPERTY()
	UImage* HourglassImage;
	
	void StartRotate();
	void StopRotate();

	float StartAngle;
	float EndAngle;
	bool bRotating;
	float IntervalTimer;
	float RotateTimer;
};
