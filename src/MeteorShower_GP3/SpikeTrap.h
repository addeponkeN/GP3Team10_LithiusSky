#pragma once

#include "CoreMinimal.h"
#include "MeteorShower_GP3Character.h"
#include "Components/BoxComponent.h"
#include "DataAssets/SpikeTrapDataAsset.h"
#include "GameFramework/Actor.h"
#include "SpikeTrap.generated.h"

UENUM()
enum ESpikeTrapState
{
	SpikeIdle UMETA(DisplayName = "Idle"),
	SpikeTriggered UMETA(DisplayName = "Triggered"),
	SpikeAttacking UMETA(DisplayName = "Attacking"),
	SpikeCooling UMETA(DisplayName = "Cooling"),
};

UCLASS()
class METEORSHOWER_GP3_API ASpikeTrap : public AActor
{
	GENERATED_BODY()

public:
	ASpikeTrap();

protected:
	virtual void BeginPlay() override;
	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

public:
	ESpikeTrapState State;

	UPROPERTY(EditAnywhere)
	USpikeTrapDataAsset* Settings;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* HitBox;


	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex);

	UFUNCTION()
	void UpdateState();

	UFUNCTION(BlueprintCallable)
	void OnTrigger();

	UFUNCTION()
	void OnAttacking();

	UFUNCTION()
	void OnCooling();

	UFUNCTION()
	void SetIdle();

	// UPROPERTY(EditAnywhere)
	// float Cooldown{3.f};
	//
	// UPROPERTY(EditAnywhere)
	// float TriggerTime{0.25f};
	//
	// UPROPERTY(EditAnywhere)
	// float AttackTime{1.5f};
	//
	// UPROPERTY(EditAnywhere)
	// float CoolTime{1.5f};

private:
	float TriggerTimer;

	UPROPERTY()
	AMeteorShower_GP3Character* Character;

	FTimerHandle UpdateTimerHandle;
	FTimerManager* TimerManager;
};
