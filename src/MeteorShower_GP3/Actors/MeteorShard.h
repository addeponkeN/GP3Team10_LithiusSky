#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeteorShard.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnImpact);

UCLASS()
class METEORSHOWER_GP3_API AMeteorShard : public AActor
{
	GENERATED_BODY()

public:
	AMeteorShard();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnImpact OnImpact;
	
	virtual void Tick(float DeltaTime) override;

	void InitShard(FVector StartLocation, FVector EndLocation);
	void UpdatePosition(float Alpha);
	void Impact();

	UFUNCTION()
	void KillShard();

private:
	FVector Start;
	FVector End;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	float LifeTimer;

	float ImpactTime{2.5f};

	FTimerHandle LifeTimerHandle;
};
