#pragma once

#include "CoreMinimal.h"
#include "SlowMoNiagara.h"
#include "TimerManager.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "DataAssets/TimeStoppableDataAsset.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"
#include "TimeStoppable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStoppedEvent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResumedEvent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLookBeginEvent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLookEndEvent);

class FTimerManager;

USTRUCT()
struct FTimeStopStatsSnapshot
{
	GENERATED_BODY()

	static FTimeStopStatsSnapshot Default;

	UPROPERTY(VisibleAnywhere)
	bool bGravityEnabled{true};

	UPROPERTY(VisibleAnywhere)
	float CustomTimeDilation{1.0f};
};

enum class AnimationStates_Time : uint8
{
	In,
	Stay,
	Out,
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class METEORSHOWER_GP3_API UTimeStoppable : public UActorComponent
{
	GENERATED_BODY()

public:
	UTimeStoppable();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual void Tick_TimeIndependent();

#pragma region Animation

	//How long it takes to transition into the time material
	UPROPERTY(EditAnywhere, Category=Animation)
	float InTransitionTime{0.45f};

	//How long it takes to transition into normal material
	UPROPERTY(EditAnywhere, Category=Animation)
	float OutTransitionTime{0.45f};

	UPROPERTY(EditAnywhere, Category=Animation)
	UCurveFloat* EasingIn;

	UPROPERTY(EditAnywhere, Category=Animation)
	UCurveFloat* EasingOut;

	AnimationStates_Time AnimationState{AnimationStates_Time::Stay};

	float TransitionTimer_Internal;

	FTimerHandle TickTimeIndependentTimerHandle;
#pragma endregion

public:
	UPROPERTY(EditAnywhere)
	UTimeStoppableDataAsset* Settings;

	UPROPERTY()
	UAudioComponent* StoppedSound;

	UPROPERTY()
	UAudioComponent* ResumedSound;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> TransitionMaterials;

	UPROPERTY(BlueprintAssignable, Category="TimeStoppable")
	FOnStoppedEvent OnStoppedEvent;

	UPROPERTY(BlueprintAssignable, Category="TimeStoppable")
	FOnResumedEvent OnResumedEvent;

	UPROPERTY(BlueprintAssignable, Category="TimeStoppable")
	FOnLookBeginEvent OnLookBeginEvent;

	UPROPERTY(BlueprintAssignable, Category="TimeStoppable")
	FOnLookEndEvent OnLookEndEvent;

	UFUNCTION(BlueprintCallable)
	void Freeze(float pSlowDuration, float pSlowAmount);

	UFUNCTION()
	void StartFreeze();

	UFUNCTION()
	void StartUnFreeze(bool ImmediateUnFreeze);

	UFUNCTION()
	void StartUnFreeze_Impl();

	UFUNCTION()
	void OnLookBegin();

	UFUNCTION()
	void OnLookEnd();

	UFUNCTION()
	void UpdateSlowAmount();

	UPROPERTY(BlueprintReadOnly)
	bool bFrozen;

	UPROPERTY()
	FTimeStopStatsSnapshot Snapshot;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FVector Angular;

	// UPROPERTY(EditAnywhere)
	// bool bFullyStoppedWhenFreeze{false};

	/**
	 * 0f = FULLY STOPPED\n
	 * 1f = NORMAL SLOWDOWN\n
	 * 2f = LESS SLOWDOWN
	 */
	UPROPERTY(EditAnywhere)
	float SlowAmountScale{1.f};

	UPROPERTY(EditAnywhere)
	bool bOnlyJuiceMeshParent{false};

	/**
	 * 1 = normal speed\n
	 * 0 = fully stopped
	 */
	float DesiredSlowAmount;
	float SlowAmountStart;
	float SlowAmountEnd;

	float GravityZ;

	/**
 * 1 = normal speed\n
 * 0 = fully stopped
 */
	float SlowAmount;

	float ProcessTime{0.3f};
	float ProcessTimer{0.f};
	float ProcessTimerMultiplier{1.f};

	bool bDoneProcess;
	bool bEntering;

	float UpdateSlowDelta{0.0334f};

private:
	void AddSlowMo(UStaticMeshComponent* Mesh);

	UFUNCTION()
	void UnFreeze();

	bool bMeshRefPhysics;

	UPROPERTY()
	UStaticMeshComponent* MeshRoot;
	
	UPROPERTY()
	TArray<UStaticMeshComponent*> ChildMeshes;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	AActor* OwnerActor;

	UPROPERTY(VisibleAnywhere)
	TArray<UNiagaraComponent*> SlowMos;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* TimeStoppableSystem;

	FTimerManager* TimerManager;
	FTimerHandle TimerHandle;
	FTimerHandle SlowAmountUpdateTimerHandle;
};
