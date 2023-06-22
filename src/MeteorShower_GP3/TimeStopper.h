
#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TimeStopper.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class METEORSHOWER_GP3_API UTimeStopper : public USceneComponent
{
	GENERATED_BODY()

public:	
	UTimeStopper();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SlowDuration{6.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SlowAmount{0.05f};
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
