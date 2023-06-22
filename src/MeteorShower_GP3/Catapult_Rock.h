// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Catapult_Rock.generated.h"

UENUM(BlueprintType)
enum class ERespawnSequenceState: uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Hiding = 1 UMETA(DisplayName = "Hiding"),
	Moving = 2 UMETA(DisplayName = "Moving"),
	Showing = 3 UMETA(DisplayName = "Showing")
};
UCLASS()
class METEORSHOWER_GP3_API ACatapult_Rock : public AActor
{
	GENERATED_BODY()
	
	void ResetRespawnTimer();
public:
	UMaterialInstanceDynamic* RockMaterial;
	UStaticMeshComponent* RootMesh;
	// Sets default values for this actor's properties
	ACatapult_Rock();
	ERespawnSequenceState RespawnSequenceState = ERespawnSequenceState::None;
	FTimerManager* TimerManger;
	FTimerHandle RespawnTimerHandle;
	float RespawnTimer = 0;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Respawn)
	float DissolveTime = 1.0f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void TRespawnTick(USkeletalMeshComponent* SkeletalMeshComponent, FString SocketName);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Initialize();
	void CoupleToSocket(USkeletalMeshComponent* SkeletalMeshComponent, FString SocketName);
	void Respawn(USkeletalMeshComponent &SkeletalMeshComponent, FString SocketName);
};
