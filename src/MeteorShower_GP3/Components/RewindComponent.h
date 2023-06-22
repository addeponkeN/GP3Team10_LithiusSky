#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RewindComponent.generated.h"

USTRUCT()
struct FRewindSnapshot
{
	GENERATED_BODY()

	FRewindSnapshot()
		: Location(FVector::Zero()), Rotation(FRotator::ZeroRotator)
	{
	}

	FRewindSnapshot(const FVector& Location, const FRotator& Rotation)
		: Location(Location), Rotation(Rotation)
	{
	}

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRewindFinishedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRewindStartedEvent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class METEORSHOWER_GP3_API URewindComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URewindComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnRewindFinishedEvent OnRewindFinishedEvent;

	UPROPERTY(BlueprintAssignable)
	FOnRewindStartedEvent OnRewindStartedEvent;

	UFUNCTION(BlueprintCallable)
	void StartRecord();

	UFUNCTION(BlueprintCallable)
	void PauseRecord();
	
	UFUNCTION(BlueprintCallable)
	void StopRecord();

	UFUNCTION()
	void TakeSnapshot();

	UFUNCTION()
	void ResetTimeline();

	UFUNCTION(BlueprintCallable)
	void SetTimelineLength(float InSecondsToRecord);
	
	UFUNCTION(BlueprintCallable)
	void StartRewind();

	UFUNCTION()
	void RewindFinished();

	UFUNCTION()
	FRewindSnapshot GenerateSnapshot() const;

	/**
	 * @brief How far the rewind can go back in time\n
	 * Zero is infinite
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SecondsToRecord{3.f};

	/**
	 * @brief How long it takes to rewind
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RewindTime{2.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecordDelta{0.5f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AutoTriggerWhenDoneRecording;

private:
	int CurrentIndex;
	int RecordSize;
	float SnapTimer;
	float TimePerSnap;
	bool bRewinding;

	FRewindSnapshot CurrentSnap;
	FRewindSnapshot NextSnap;

	UPROPERTY()
	AActor* Owner;

	UPROPERTY()
	AController* Controller;

	UPROPERTY()
	TArray<FRewindSnapshot> Timeline;

	FTimerHandle SnapshotTimerHandle;
	FTimerManager* TimerManager;
};
