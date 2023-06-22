#pragma once

#include "CoreMinimal.h"
#include "TimeStoppable.h"
#include "Components/SceneComponent.h"
#include "DataAssets/TimeGunDataAsset.h"
#include "TimeGun.generated.h"

class AMeteorShower_GP3Character;
class ATimeGunLaser;
class UArrowComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShootEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeStoppedTargetEvent, UTimeStoppable*, HitTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeUnstoppedTargetEvent, UTimeStoppable*, HitTarget);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class METEORSHOWER_GP3_API UTimeGun : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UTimeGun();

protected:
	virtual void BeginPlay() override;
	virtual void OnComponentCreated() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTimeGunDataAsset* Settings;
	
	UPROPERTY(BlueprintAssignable)
	FOnShootEvent OnShootEvent;

	UPROPERTY(BlueprintAssignable)
	FOnTimeStoppedTargetEvent OnTimeStoppedTargetEvent;

	UPROPERTY(BlueprintAssignable)
	FOnTimeUnstoppedTargetEvent OnTimeUnstoppedTargetEvent;

	UFUNCTION()
	void StartLineTraceTimeStoppableObjects();

	UFUNCTION()
	void ClearLookTarget();

	UFUNCTION(BlueprintCallable)
	void Shoot();

	UFUNCTION(BlueprintCallable)
	void AttachToCharacter(AMeteorShower_GP3Character* Target);

	UFUNCTION()
	void FreezeTarget(UTimeStoppable* StoppableTarget);

	UPROPERTY()
	AMeteorShower_GP3Character* Character;

	UPROPERTY()
	UTimeStoppable* CurrentStoppedObject;

	UPROPERTY()
	UTimeStoppable* CurrentLookTarget;

	UPROPERTY()
	UArrowComponent* MuzzleLocation;
	
private:
	UFUNCTION()
	void LineTraceStoppableObjects();
	
	UFUNCTION()
	bool HandleHitResult(FHitResult& Result);

	UFUNCTION()
	void ClearFreezeTarget();

	FTimerHandle ClearCurrentStoppableObjectTimerHandle;
	FTimerHandle TracerTimerHandle;
};
