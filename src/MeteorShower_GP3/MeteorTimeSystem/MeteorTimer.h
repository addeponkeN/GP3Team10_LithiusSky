#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeteorShower_GP3/DataAssets/MeteorTimerDataAsset.h"
#include "MeteorTimer.generated.h"

class IMeteorTimed;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimerChangedEvent, AMeteorTimer*, MeteorTimer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimerStoppedEvent, AMeteorTimer*, MeteorTimer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimerStartedEvent, AMeteorTimer*, MeteorTimer);

UCLASS()
class METEORSHOWER_GP3_API AMeteorTimer : public AActor
{
	GENERATED_BODY()

public:
	AMeteorTimer();

protected:
	virtual void BeginPlay() override;

public:
	
	// UFUNCTION(BlueprintCallable, Category="MeteorTimer")
	// void AddSoundTrigger(USoundBase* Sound, float TimeLeftWhenToPlay, bool PlayOncePerGameSession = false);
	
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, Category="MeteorTimer")
	FTimerChangedEvent OnTimerChangedEvent;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, Category="MeteorTimer")
	FTimerStoppedEvent OnTimerStoppedEvent;
	
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, Category="MeteorTimer")
	FTimerStartedEvent OnTimerStartedEvent;

	UFUNCTION(BlueprintCallable, Category="MeteorTimer")
	void SetTimerEnabled(bool EnableTimer);

	UFUNCTION(BlueprintCallable, Category="MeteorTimer")
	void SetTimer(float Value);

	UFUNCTION(BlueprintCallable, Category="MeteorTimer")
	void StartTimer();

	UFUNCTION(BlueprintCallable, Category="MeteorTimer")
	void StopTimer();

	UFUNCTION(BlueprintCallable, Category="MeteorTimer")
	void PauseTimer();

	UFUNCTION(BlueprintCallable, Category="MeteorTimer")
	float GetTimer() const { return Timer; }

	UFUNCTION(BlueprintCallable)
	FString GetFormattedString();

	/**
	 * @brief 0=Meteor impact,  1=Meteor falling
	 * @return Returns the progress of the timer (0.f - 1.f)
	 */
	UFUNCTION(BlueprintCallable, Category="MeteorTimer")
	float GetPercent() const { return Timer / MeteorImpactTime; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MeteorTimer")
	float MeteorImpactTime{20.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MeteorTimer")
	float TimerTickRate{0.03223f};

	UPROPERTY(EditAnywhere, Category="MeteorTimer")
	UMeteorTimerDataAsset* Settings;

	bool bTimerEnabled;

private:
	template <class T>
	void AddMeteorTimedComp(FName ComponentName);

	UFUNCTION()
	void DecreaseTimer();

	UPROPERTY(EditAnywhere)
	float Timer;

	FTimerHandle IncrementTimerHandle;
};

template <class T>
void AMeteorTimer::AddMeteorTimedComp(FName ComponentName)
{
	auto MeteorTimedComponent = CreateDefaultSubobject<T>(ComponentName);
	AddOwnedComponent(MeteorTimedComponent);
}
