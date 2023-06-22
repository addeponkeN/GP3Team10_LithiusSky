#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "CharacterHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKilledEvent);

/*
 *	Derived from BoxCollider/Component
 *	Adjust the extents as this will be the collider that collides with DamageZones/KillZones
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class METEORSHOWER_GP3_API UCharacterHealthComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UCharacterHealthComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnKilledEvent OnKilledEvent;

	UFUNCTION(BlueprintCallable, Category="Health")
	void Kill();

	/**
	 * @brief Kills the Player If Player Location Z is below KillZ
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float KillZ{-2000.f};

private:
	UFUNCTION()
	void CheckCharacterBounds();

	FTimerHandle BoundsTimerHandle;
};
