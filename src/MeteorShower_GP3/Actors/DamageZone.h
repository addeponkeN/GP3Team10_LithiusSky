#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "DamageZone.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamagedTargetEvent, AActor*, DamagedActor);

UCLASS()
class METEORSHOWER_GP3_API ADamageZone : public AActor
{
	GENERATED_BODY()

public:
	ADamageZone();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FOnDamagedTargetEvent OnDamagedTargetEvent;

	UPROPERTY(EditAnywhere)
	float Damage{1.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* Area;

private:
	UFUNCTION()
	void OnOverlappedTarget(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
