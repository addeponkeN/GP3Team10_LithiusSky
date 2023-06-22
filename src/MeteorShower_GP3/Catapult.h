// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Catapult_Rock.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Actor.h"
#include "Catapult.generated.h"

UCLASS()
class METEORSHOWER_GP3_API ACatapult : public AActor, public IInteractable
{
	GENERATED_BODY()
private:
	void TriggerSwingAnimation();
	// Sets default values for this actor's properties
	ACatapult();

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Rock)
	TSubclassOf<ACatapult_Rock> CatapultRock;
	ACatapult_Rock* SpawnedRock;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditDefaultsOnly, Category= Animation)
	UAnimInstance* RockThrowAnimInstance;
	UPROPERTY(EditDefaultsOnly, Category= Animation)
	UAnimMontage *AnimMontage;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void RespawnRock();
	UFUNCTION()
	void SetUpRock();
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Values)
	float RockRespawnTime = 10;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Values)
	float ImpulseForce = 200000;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Values)
	float LaunchHeight = 0.5;
	FTimerHandle RockFlyingTimerHandle;
	UFUNCTION(BlueprintCallable)
	FVector GetLaunchVector();
public:
	UFUNCTION()
	virtual void Interact() override;
	UFUNCTION(BlueprintCallable)
	void LaunchRock();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
