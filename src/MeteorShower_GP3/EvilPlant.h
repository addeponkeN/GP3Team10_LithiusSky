#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Util/Pooling/Poolable.h"
#include "EvilPlant.generated.h"

UCLASS()
class METEORSHOWER_GP3_API AEvilPlant : public APoolable
{
	GENERATED_BODY()

public:
	AEvilPlant();

protected:
	virtual void BeginPlay() override;

public:
	void Push();

	UFUNCTION()
	void SpawnPlant();

	UFUNCTION()
	void KillPlant();
	
	UPROPERTY()
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APoolable> ClassTemplate;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APoolable> ClassTemplate2;

	UPROPERTY()
	FTimerHandle handle;

	UPROPERTY()
	FTimerHandle spawnTimerHandle;

	UPROPERTY()
	APoolable* spawned;
};
