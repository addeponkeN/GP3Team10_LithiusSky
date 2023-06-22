#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeteorShower_GP3/TimeStoppable.h"
#include "Curves/CurveFloat.h"
#include "BlinkerComponent.generated.h"


enum class  AnimationStates : uint8
{
	In,
	StayVisible,
	Out,
	StayHidden,
};
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class METEORSHOWER_GP3_API UBlinkerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UBlinkerComponent();

protected:
	virtual void BeginPlay() override;
	AnimationStates AnimationState{AnimationStates::StayVisible};
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	UMaterialInstanceDynamic* TransitionMaterial;
	
	UFUNCTION()
	void StartBlinking();

	UPROPERTY(BlueprintReadOnly)
	bool bVisible;

	/*//How long the object will be visible
	UPROPERTY(EditAnywhere, Category=Blinker)
	float VisibleTime{2.f};

	//How long the object will be hidden
	UPROPERTY(EditAnywhere, Category=Blinker)
	float HiddenTime{2.f};*/

	//How long the platform waits while visible before transitioning
	UPROPERTY(EditAnywhere, Category=Animation)
	float VisibleStayTime = 1.5f;
	//How long the platform waits while Hidden before transitioning
	UPROPERTY(EditAnywhere, Category=Animation)
	float HiddenStayTime = 1.5f;
	
	#pragma region Animation 

	//How long it takes to transition from Visible to Hidden
	UPROPERTY(EditAnywhere, Category=Animation)
	float HiddenTransitionTime{1.0f};

	//How long it takes to transition from Hidden to Visible
	UPROPERTY(EditAnywhere, Category=Animation)
	float VisibleTransitionTime{1.0f};

	UPROPERTY(EditAnywhere, Category=Animation)
	UCurveFloat* EasingInVisible;
	UPROPERTY(EditAnywhere, Category=Animation)
	UCurveFloat* EasingOutHidden;
	bool bAllowColliderOnTransitionHidden = true;
	#pragma endregion

private:
	void SetIsVisible(bool IsVisible);
	
	void SetStartVisible();
	void SetStateVisible();

	void SetStartHidden();
	void SetStateHidden();

	void SetTransitionMaterialValue(float Value);

	UPROPERTY()
	TArray<UStaticMeshComponent*> Meshes;

	float timer;

	UPROPERTY()
	AActor* OwnerActor;
	UPROPERTY()
	UTimeStoppable* TimeStoppable;
};
