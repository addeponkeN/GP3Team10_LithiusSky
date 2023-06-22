#pragma once

#include "CoreMinimal.h"
#include "Laser.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "TimeGunLaser.generated.h"

UCLASS()
class METEORSHOWER_GP3_API ATimeGunLaser : public ALaser
{
	GENERATED_BODY()

public:
	ATimeGunLaser();

protected:
	virtual void BeginPlay() override;
	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

public:
	virtual void SetDestination(FVector const& Destination) override;

	UPROPERTY()
	ULineBatchComponent* Line;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NiagaraSystem;

private:
	FVector GetBeamEnd();
	FString BeamVariableEndName{FString(TEXT("BeamEnd"))};
	FVector DestinationLocation;
};
