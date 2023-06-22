// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CableComponent.h"
#include "GrapplingHookObject.h"
#include "Components/GrappableOverlay.h"
#include "DataAssets/GrappleSettings.h"
#include "GrappelingHookComponent.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateChange);

class AMeteorShower_GP3Character;
class UCurveFloat;
class UGrappleSettings;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class METEORSHOWER_GP3_API UGrappelingHookComponent : public USceneComponent
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=HookValues, meta=(AllowPrivateAccess = "true"))
	float MaxHitRange = 10000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=HookValues, meta=(AllowPrivateAccess = "true"))
	UCurveFloat* reelInEasing;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	//* Sound
	#pragma region sound
	
	UPROPERTY(EditAnywhere, Category=Sound)
	USoundBase* GrappleReel;

	UPROPERTY()
	UAudioComponent* GrappleReelAudioComponent;

	#pragma endregion
	
	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;
	
	UPROPERTY(EditAnywhere, Category="Collision")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty;
	UPROPERTY(EditAnywhere, Category="Collision")
	TEnumAsByte<ECollisionChannel> DetachTraceChannelProperty;
	UPROPERTY(EditAnywhere,Category = "HookValues")
	float ReelInStrength = 2000.0f;
	UPROPERTY(EditAnywhere,Category = "HookValues")
	float TimeBeforeTerminalVelocity = 0.5f;
	float DeatachAngleMargin = -0.1f;
	float LookAwayDetachMargin = -0.2f;
	UPROPERTY(EditAnywhere,Category = "HookValues")
	float DeatachDistance = 150.0f;
	UPROPERTY(EditAnywhere,Category = "HookValues")
	//How much the velocity of the player pulls against the rope
	float ReverseAccelerationMultiplier = 1;
	//How big our hit scan is, if its higher then its easier to aim
	UPROPERTY(EditAnywhere,Category = "HookValues")
	float SphereTraceRadius = 150;
	FVector AttachedPoint = FVector::Zero();
	FVector AttachedNormal = FVector::Zero();
	float TimeSinceAttached = 0;
	
public:	
	// Sets default values for this component's properties
	UGrappelingHookComponent();

protected:

	UPROPERTY()
	UGrappleSettings* Settings;

#pragma region Audio
	UPROPERTY()
	UAudioComponent* ReelAudioComponent;
#pragma endregion
	
#pragma region Cable
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cable_Internal, meta = (AllowPrivateAccess = "true"))
	UCableComponent* Cable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cable_Internal, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGrapplingHookObject> HookObject;
	UPROPERTY()
	TObjectPtr<AGrapplingHookObject> CableAttachActor;
	FName RopeAttachSocket = "Rope";
	void UpdateCableEndLocation(FVector AttachPoint);
	void SetUpCable();
	
#pragma endregion
	
#pragma region Cable Internal
	
	UFUNCTION()
	void TAnimateCableHookAbject();
	void StartCableHookAnimation();
	FTimerHandle HookAnimationTimerHandle;
	UPROPERTY(BlueprintReadOnly)
	bool AnimatingHook = false;
	float HookAnimationTimer = 0;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Hook)
	float HookAnimationTime = 0.4f;
	float AdjustedHookAnimationTime = 0.4f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Hook)
	UCurveFloat* HookAnimationCurve;
	FVector HookStartPosition;
	FVector HookPath; 
	
#pragma endregion

#pragma region OnLookEvents

	FTimerHandle ProcessOnLookEventsTimerHandle;
	float OnLookEventsTickRate = 0.1f;
	UPROPERTY()
	UGrappableOverlay* CurrentOnLookTarget;
	UFUNCTION()
	void TProcessOnLookEvents();
	void StartProcessingOnLookEvents();
	void ClearOnLookTarget();
	
#pragma endregion

#pragma region Animations
	void TriggerShootAnimation();
	UFUNCTION(BlueprintCallable)
	void OnShootFinished();
	void TriggerReelInAnimation();
	UFUNCTION(BlueprintCallable)
	void OnReelInAnimationFinished();
	bool ShootAnimInProgress = false;
#pragma endregion 
protected:
	UPROPERTY()
	AMeteorShower_GP3Character* Character;
	UPROPERTY()
	APlayerController* PlayerController;
	
	FTimerHandle AttachGrappleHandle;
	FTimerManager* TimerManager;
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void Fire();
	FVector GetStartAttachPoint();
	FVector GetGrappleTravelDirection();
	bool ShouldDeattach();
	void HandleDeattach();
	virtual bool ValidateFireLocation(FHitResult &Out);
	virtual void AttachGrapplePoint(FVector &AttachPoint);
	virtual void ToggleHook();
	float GetReverseMultiplier(FVector DesiredDirection,FVector CurrentVelocity);
	UFUNCTION()
	void THookAttackedTick(FVector AttachPoint, FVector StartVelocity);

	UFUNCTION(BlueprintImplementableEvent,Category = Events)
	void OnGrappleStart();
	UFUNCTION(BlueprintImplementableEvent,Category = Events)
	void OnGrappleEnd();
	/*UPROPERTY(BlueprintAssignable,Category = Events)
	FOnStateChange OnGrappleStart_Delegate;
	UPROPERTY(BlueprintAssignable,Category = Events)
	FOnStateChange OnGrappleEnd_Delegate;*/
	void OnGrappleStart_Internal();
	void OnGrappleEnd_Internal();
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
    void AttachHook(AMeteorShower_GP3Character* TargetCharacter);
	void AttachGrapplingMeshToTimeGunMesh(AMeteorShower_GP3Character* TargetCharacter);
	void QueryForGrapplingHookMesh();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* GrapplingGunMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh)
	FVector SizeOnAttach = FVector(0.2,0.2,0.2);
	FName GrapplingHookAttachSocketName = "GrappleAttachment";
	FName FireLocationSocketName = "Muzzle";
	FString TimeGunMeshName = "TimeGun";
	FString GrapplingGunMeshName = "GrapplingGunMesh";
	
	UPROPERTY(BlueprintReadOnly)
	bool bHasBeenPickedUp = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool bInUse;
};
