// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "CheckpointManager.generated.h"

class AGameSubStateManager;
class ACheckpoint;
class AGigaCheckpoint;
class UCheckpointManagerDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivatedCheckpoint, ACheckpoint*, Checkpoint);

/*
 *	Derived from SphereComponent
 *	Adjust the Sphere Radius as it will be the thing that collides with Checkpoints
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class METEORSHOWER_GP3_API UCheckpointManager : public USphereComponent
{
	GENERATED_BODY()

public:
	UCheckpointManager();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                          const FHitResult& SweepResult);

public:
	UPROPERTY(EditAnywhere)
	UCheckpointManagerDataAsset* Settings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACheckpoint* CurrentCheckpoint;

	UPROPERTY()
	ACheckpoint* FallbackCheckpoint;

	void QuickRestart();
	void GigaRestart();
	void OnRestart(bool IsGigaRestart);
	bool CanRespawn();

	UFUNCTION()
	void ClearRespawnCooldown();

	//sets the current checkpoint to the fallback checkpoint
	void ClearCurrentCheckpoint();

	UFUNCTION(BlueprintCallable)
	void RespawnAtCheckpoint();

	UFUNCTION(BlueprintCallable)
	void SetCheckpoint(ACheckpoint* TargetCheckpoint);

	UFUNCTION(BlueprintCallable)
	void SetGigaCheckpoint(AGigaCheckpoint* TargetGigaCheckpoint);

	UFUNCTION(BlueprintCallable)
	void SetFallbackGigaCheckpoint(AGigaCheckpoint* TargetGigaCheckpoint);

	UPROPERTY(BlueprintReadOnly)
	FOnActivatedCheckpoint OnActivatedCheckpointEvent;

	UPROPERTY(VisibleAnywhere)
	AGigaCheckpoint* GigaCheckpoint;

	UPROPERTY(VisibleAnywhere)
	AGigaCheckpoint* FallbackGigaCheckpoint;

private:
	bool bCanRespawn{true};
	float RespawnCooldown{2.f};
	FTimerHandle CooldownTimerHandle;
	
	// UPROPERTY()
	// AGameSubStateManager* StateManager;
};
