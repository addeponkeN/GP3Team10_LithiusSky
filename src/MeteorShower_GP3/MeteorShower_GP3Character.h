// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GrappelingHookComponent.h"
#include "Components/CharacterHealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "TimeGun.h"
#include "CheckpointSystem/CheckpointManager.h"
#include "Ui/GameHud.h"
#include "MeteorShower_GP3Character.generated.h"

class UCharacterSettings;
class UGameSettingsDataAsset;
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLook);

struct FWallClimbParams
{
	AActor* Actor;
	FVector StartPosition;
	FVector TargetPosition;
	FVector StartOffset;
	FVector EndOffset;
};

UCLASS(config=Game)
class AMeteorShower_GP3Character : public ACharacter
{
	GENERATED_BODY()
	

	
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	
	// UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	// USpringArmComponent* SpringArmComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	/** Dash Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* EdgeGrabAction;

public:
	AMeteorShower_GP3Character();
protected:
	
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;

public:
	UPROPERTY()
	UCharacterSettings* Settings;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnGrappleStart();
	UFUNCTION(BlueprintImplementableEvent)
	void OnGrappleEnd();
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GrapplingHook)
	bool bHasGrapplingHook;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GrapplingHook)
	UGrappelingHookComponent* GrapplingHook;
	UPROPERTY(BlueprintAssignable, Category= OnLookEvents)
	FOnLook OnLookBegin;
	UPROPERTY(BlueprintAssignable, Category= OnLookEvents)
	FOnLook OnLookEnd;


	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

protected:
	float CapsuleHeight;
	float CapsuleWidth;
	float CapsuleHeight_WithoutHemisphere;
	float CapsuleWidth_WithoutHemisphere; 
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	virtual void Landed(const FHitResult& Hit) override;
#pragma region EdgeGrab
	UPROPERTY(EditAnywhere,Category= EdgeGrab)
	TEnumAsByte<ECollisionChannel> CollisionChannel;
	
	UPROPERTY(EditAnywhere,Category= EdgeGrab)
	float EdgeGrabDistanceToWall = 20;
	
	//Some edges are round, how far should we offset to find flat ground
	UPROPERTY(EditAnywhere,Category= EdgeGrab)
	float RoundLedgeTolerance = 20;
	
	UPROPERTY(EditAnywhere,Category= EdgeGrab)
	//How different the angle can be from the upvector 0 == Flat 1 == 90 degrees; 
	float EdgeGrabNormalizedAngleLimit = 0.2f;
	
	//Between what heights edgegrabbing should be allowed measured from the middle of the character
	UPROPERTY(EditAnywhere,Category= EdgeGrab)
	float EdgeGrabHeight_Min = 70;
	
	UPROPERTY(EditAnywhere,Category = EdgeGrab)
	float EdgeGrabHeight_Max = 120;
	UPROPERTY(EditAnywhere,Category = EdgeGrab)
	float WallClimbTime = 1.0f;
	//How fast the character gets moved onto the platform
	UPROPERTY(EditAnywhere, Category = EdgeGrab)
	float FloorClimbTime = 0.2f;
	UPROPERTY(EditAnywhere, Category = EdgeGrab)
	UCurveFloat* WallClimbTimeCurve;
	UPROPERTY(EditAnywhere, Category = EdgeGrab)
	bool DrawEdgeGrabDebugLines;
	FTimerHandle EdgeClimbTimerHandle;
	float WallClimbTimer = 0;
	float FloorClimbTimer = 0;
	UFUNCTION(BlueprintCallable)
	void TryEdgeGrab();
	bool CanEdgeGrab(FWallClimbParams &WallClimbParams);
	void InitializeEdgeClimb(FWallClimbParams &WallClimbParams);
	void TClimbEdge(FWallClimbParams WallClimbParams);
	virtual void Jump() override;
#pragma endregion

#pragma region ImpactSound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ImpactSound)
	float MinImpactSoundMultiplier = 0.1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ImpactSound)
	float MaxImpactSoundMultiplier = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ImpactSound)
	float MaxVelocityBound = 3000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ImpactSound)
	float MinVelocityBound = 400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ImpactSound)
	UCurveFloat* ImpactSoundDistributionCurve;
	FTimerHandle ImpactAudioTimerHandle;
#pragma endregion 

#pragma region CoyoteTime
	
	float BaseJumpForce;
	void Jump_CoyoteTime();
	void StartCoyoteTime();
	UFUNCTION()
	void TOnCoyoteTimerTick();
	void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	UPROPERTY(EditAnywhere, Category = Jump)
	float CoyoteTime = 0.3f;
	float CoyoteTimer_Internal = 0;
	FTimerHandle CoyoteTimeTimerHandle;
	FTimerManager* TimerManager;
	
#pragma endregion
	
#pragma region Dash
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Dash)
	bool bHasDash = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Dash)
	bool bCanDash = true;
	UPROPERTY(EditAnywhere,Category = Dash)
	float DashLength = 700.0f;
	UPROPERTY(EditAnywhere,Category = Dash)
	float DashCooldown = 1.2f;
	UFUNCTION(BlueprintCallable)
	float GetDashCooldownElapsed();
	UFUNCTION(BlueprintCallable)
	float GetDashCooldown();
	UFUNCTION(BlueprintCallable)
	void ResetDash();
	UFUNCTION(BlueprintCallable)
	void CancelDash();
	//The dash Speed 
	UPROPERTY(EditAnywhere,Category = Dash)
	float DashSpeed = 1;
	UPROPERTY(EditAnywhere,Category = Dash)
	UCurveFloat* DashEasingCurve;
	//How far down we can raycast to predict the position of where you want to land.
	UPROPERTY(EditAnywhere,Category = Dash)
	float AllowedHeightMargin = 100;
	//How far in you'll land on a platform that you're trying to dash to
	UPROPERTY(EditAnywhere,Category = Dash)
	float DashToLedgeMargin = 40;
	float DashTimer_Internal = 0;
	UPROPERTY(EditAnywhere,Category = Dash)
	bool EnableDashDebugLines = false;
	FTimerHandle DashTimerHandle;
	FTimerHandle DashCooldownTimerHandle;
	void Dash();
	//This is triggered when the dash helps a user over a ledge, when looking from below
	UFUNCTION(BlueprintImplementableEvent)
	void OnPredictedEdgeLand();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDashStart_Implementable();
	void OnDashStart();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDashEnd_Implementable();
	void OnDashEnd();
	UFUNCTION()
	void TDashTimerTick(FVector StartPosition,FVector PredictedPosition);
	FVector GetPredictedDashDestination();
#pragma endregion
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	FHitResult GetGroundHit();
	UFUNCTION(BlueprintCallable)
	FVector GetGroundForward();
	UFUNCTION(BlueprintCallable)
	FVector GetAlignedDashVector();
	UFUNCTION(BlueprintCallable)
	void SetInputEnabled(bool Enabled);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category="HUD")
	UGameHud* GameHud;

	UPROPERTY(EditAnywhere, Category="HUD")
	TSubclassOf<UGameHud> GameHudClass;

	UPROPERTY(VisibleAnywhere, Category="Health")
	UCharacterHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, Category="Checkpoint")
	UCheckpointManager* CheckpointManager;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// float MouseSensitivity{1.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	UGameSettingsDataAsset* GameSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimeGun* TimeGun;
};
