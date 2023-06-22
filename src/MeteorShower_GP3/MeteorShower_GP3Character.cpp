// Copyright Epic Games, Inc. All Rights Reserved.

#include "MeteorShower_GP3Character.h"

#include <string>

#include "MeteorShower_GP3Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MeteorShower_GP3.h"
#include "SoundHelper.h"
#include "DataAssets/CharacterSettings.h"
#include "DataAssets/GameSettingsDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"


//////////////////////////////////////////////////////////////////////////
// AMeteorShower_GP3Character

AMeteorShower_GP3Character::AMeteorShower_GP3Character()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	//Create a SpringArmComponent
	// SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	// SpringArmComponent->SetupAttachment(GetCapsuleComponent());

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	auto SocketName = TEXT("GunSocket");
	TimeGun = CreateDefaultSubobject<UTimeGun>(TEXT("TimeGun"));
	// TimeGun->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	// TimeGun->SetupAttachment(Mesh1P, SocketName);

	//	add health component
	HealthComponent = CreateDefaultSubobject<UCharacterHealthComponent>(TEXT("Health"));
	HealthComponent->SetupAttachment(GetCapsuleComponent());

	//	add checkpoint collider
	CheckpointManager = CreateDefaultSubobject<UCheckpointManager>(TEXT("CheckpointCollider"));
	CheckpointManager->SetupAttachment(GetCapsuleComponent());

	const wchar_t* DataAssetPath = TEXT("/Game/Settings/DA_CharacterSettings.DA_CharacterSettings");
	static ConstructorHelpers::FObjectFinder<UCharacterSettings>CharacterSettings(DataAssetPath);
	
	if (CharacterSettings.Succeeded())
	{
		Settings = CharacterSettings.Object;
	}
	else
	{
		LOGWF("Could not find Character Settings Data Asset");
	}


}

void AMeteorShower_GP3Character::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	if(Settings->OnLandSound && !GetWorldTimerManager().TimerExists(ImpactAudioTimerHandle))
	{
		float ImpactScalar = FMath::Abs(Hit.Normal.Dot(GetCharacterMovement()->Velocity));
		ImpactScalar = FMath::Clamp(ImpactScalar,MinVelocityBound,MaxVelocityBound);
		
		float VolumeDelta = MaxImpactSoundMultiplier - MinImpactSoundMultiplier;
		float EasedImpactScalar = (ImpactSoundDistributionCurve!= nullptr) ?
			                      ImpactSoundDistributionCurve->GetFloatValue(ImpactScalar/MaxVelocityBound):
								  ImpactScalar/MaxVelocityBound;
		float HitVolume = MinImpactSoundMultiplier + (VolumeDelta * (EasedImpactScalar));

		USoundHelper::PlaySoundSFX(this, Settings->OnLandSound,HitVolume);
		
		GetWorldTimerManager().SetTimer(ImpactAudioTimerHandle,GetWorld()->DeltaTimeSeconds,false,Settings->OnLandSound->Duration);
	}
}

void AMeteorShower_GP3Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	BaseJumpForce = GetCharacterMovement()->JumpZVelocity;
	//Get timer Manager
	TimerManager = &GetWorld()->GetTimerManager();
	GetCapsuleComponent()->GetScaledCapsuleSize(CapsuleWidth,CapsuleHeight);
	GetCapsuleComponent()->GetScaledCapsuleSize_WithoutHemisphere(CapsuleWidth_WithoutHemisphere,CapsuleHeight_WithoutHemisphere);
	auto SocketName = TEXT("GunSocket");
	TimeGun->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	TimeGun->SetRelativeRotation(FRotator(80,180,0));
}

void AMeteorShower_GP3Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//	setup gamehud
	if (GameHudClass)
	{
		GameHud = CreateWidget<UGameHud>(Cast<APlayerController>(Controller), GameHudClass);
		if (GameHud)
		{
			GameHud->AddToPlayerScreen();
			GameHud->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AMeteorShower_GP3Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMeteorShower_GP3Character::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this,
		                                   &AMeteorShower_GP3Character::Jump_CoyoteTime);
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
		                                   &AMeteorShower_GP3Character::Move);
		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
		                                   &AMeteorShower_GP3Character::Look);
		EnhancedInputComponent->BindAction(EdgeGrabAction, ETriggerEvent::Triggered, this,
										   &AMeteorShower_GP3Character::TryEdgeGrab);
	}
}
#pragma region CoyoteTime
void AMeteorShower_GP3Character::TOnCoyoteTimerTick()
{
	CoyoteTimer_Internal += GetWorld()->DeltaTimeSeconds;
	if (TimerManager->GetTimerElapsed(CoyoteTimeTimerHandle) > CoyoteTime)
	{
		TimerManager->ClearTimer(CoyoteTimeTimerHandle);
		CoyoteTimer_Internal = 0;
	}
}

void AMeteorShower_GP3Character::StartCoyoteTime()
{
	CoyoteTimer_Internal = 0;
	TimerManager->ClearTimer(CoyoteTimeTimerHandle);
	TimerManager->SetTimer(CoyoteTimeTimerHandle, this, &AMeteorShower_GP3Character::TOnCoyoteTimerTick,
	                       GetWorld()->DeltaTimeSeconds, true, -1);
}

void AMeteorShower_GP3Character::Jump_CoyoteTime()
{
	if (CoyoteTimer_Internal < CoyoteTime && TimerManager->TimerExists(CoyoteTimeTimerHandle))
	{
		GetCharacterMovement()->Launch((GetActorForwardVector() + FVector::UpVector) * BaseJumpForce);
		TimerManager->ClearTimer(CoyoteTimeTimerHandle);
	}
	else if(GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		//If not coyote jump we want to dash, but its binded onto the same Key
		Dash();
	}
}

void AMeteorShower_GP3Character::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	if ((GetCharacterMovement()->MovementMode == MOVE_Falling && (PrevMovementMode == MOVE_Walking)) && !bPressedJump)
	{
		StartCoyoteTime();
	}
}
#pragma endregion
#pragma region EdgeGrab
void AMeteorShower_GP3Character::TryEdgeGrab()
{
	FWallClimbParams WallClimbParams;
	if (CanEdgeGrab(WallClimbParams) && !TimerManager->TimerExists(EdgeClimbTimerHandle))
	{
		InitializeEdgeClimb(WallClimbParams);
		bCanDash = true;
		ResetDash();
	}
}

void AMeteorShower_GP3Character::InitializeEdgeClimb(FWallClimbParams& WallClimbParams)
{
	WallClimbTimer = 0;
	FloorClimbTimer = 0;
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	SetInputEnabled(false);
	SetActorEnableCollision(false);
	if (TimerManager)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &AMeteorShower_GP3Character::TClimbEdge, WallClimbParams);
		TimerManager->SetTimer(EdgeClimbTimerHandle, Delegate, GetWorld()->DeltaTimeSeconds, true, -1);
	}
}

void AMeteorShower_GP3Character::TClimbEdge(FWallClimbParams WallClimbParams)
{

	WallClimbTimer += TimerManager->GetTimerElapsed(EdgeClimbTimerHandle);
	
	float CapsuleHeigt_Temp = CapsuleHeight;
	float CapsuleWidth_Temp = CapsuleWidth;
	
	const FVector VerticalClimb = (WallClimbParams.StartPosition + FVector::UpVector * (WallClimbParams.TargetPosition.Z
		+ CapsuleHeigt_Temp - WallClimbParams.StartPosition.Z)) - WallClimbParams.StartPosition;
	
	const FVector OffsetStartPosition = WallClimbParams.Actor->GetActorLocation() + WallClimbParams.StartOffset - GetActorForwardVector() * CapsuleWidth_Temp;

	const FVector FloorDeltaVector = WallClimbParams.EndOffset - (WallClimbParams.StartOffset + (FVector::UpVector * (WallClimbParams.EndOffset.Z - WallClimbParams.StartOffset.Z)));
	
	const FVector FloorMoveDirection = FloorDeltaVector.GetSafeNormal() * CapsuleWidth * 2;
	
	if (WallClimbTimer <= WallClimbTime)
	{
		const float Multiplier = (WallClimbTimeCurve != nullptr)
			                         ? WallClimbTimeCurve->GetFloatValue(WallClimbTimer / WallClimbTime)
			                         : WallClimbTimer / WallClimbTime;
		SetActorLocation(OffsetStartPosition  + (VerticalClimb * Multiplier));
	}
	else if(FloorClimbTimer <= FloorClimbTime)
	{
		FloorClimbTimer += TimerManager->GetTimerElapsed(EdgeClimbTimerHandle);
		const float Multiplier =  FloorClimbTimer / FloorClimbTime;
		SetActorLocation(OffsetStartPosition + VerticalClimb + (FloorMoveDirection * Multiplier));
	}
	else
	{
		SetActorLocation(OffsetStartPosition + VerticalClimb + FloorMoveDirection);
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		TimerManager->ClearTimer(EdgeClimbTimerHandle);
		SetInputEnabled(true);
		SetActorEnableCollision(true);
	}
}


bool AMeteorShower_GP3Character::CanEdgeGrab(FWallClimbParams& WallClimbParams)
{
	//Check if we hit a wall
	bool HeightInRange = false;
	bool AllowedAngle = false;
	FHitResult WallHit;
	float TraceRadius = 12.0f;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	ActorsToIgnore.Add(this);
	bool HitWall = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * EdgeGrabDistanceToWall,
		12.0f,
		UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		WallHit,
		true,
		FLinearColor::Green,
		FLinearColor::Red,
		GetWorld()->DeltaTimeSeconds
	);
	if (!HitWall) return false;
	FHitResult FloorHit;
	bool HitFloor;
	if (HitWall)
	{
		//Get the information from the top of the wall, check if it has a surface 
		float HeightOffset = 300;
		FVector TraceEnd = WallHit.Location + GetActorForwardVector() * RoundLedgeTolerance;
		FVector TraceStart = TraceEnd + FVector::UpVector * HeightOffset;
		HitFloor = UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(),
			TraceStart,
			TraceEnd,
			12.0f,
			UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForOneFrame,
			FloorHit,
			true,
			FLinearColor::Green,
			FLinearColor::Red,
			GetWorld()->DeltaTimeSeconds
		);

		if (HitFloor)
		{
			//Check the height of the ledge
			FVector GrabPositionMin = WallHit.Location + FVector::UpVector * EdgeGrabHeight_Min;
			FVector GrabPositionMax = WallHit.Location + FVector::UpVector * EdgeGrabHeight_Max;
			HeightInRange = (FloorHit.Location.Z > GrabPositionMin.Z && FloorHit.Location.Z < GrabPositionMax.Z);
			//Check the angle of the surface using the dot product
			float AngleOfFloorDot = FVector::UpVector.Dot(FloorHit.Normal);
			AllowedAngle = AngleOfFloorDot > 1 - EdgeGrabNormalizedAngleLimit;

			DrawDebugLine(GetWorld(),
			              GrabPositionMin,
			              GrabPositionMax,
			              (HeightInRange && AllowedAngle) ? FColor::Green : FColor::Red,
			              false,
			              GetWorld()->DeltaTimeSeconds * 2,
			              0,
			              10.0f
			);
			WallClimbParams.Actor = FloorHit.GetActor();
			WallClimbParams.StartPosition = WallHit.Location;
			WallClimbParams.TargetPosition =  FloorHit.Location;
			WallClimbParams.StartOffset = WallClimbParams.StartPosition - WallClimbParams.Actor->GetActorLocation();
			WallClimbParams.EndOffset = WallClimbParams.TargetPosition - WallClimbParams.Actor->GetActorLocation();
		}
	}
	return HitWall && HitFloor && AllowedAngle && HeightInRange;
}

void AMeteorShower_GP3Character::Jump()
{
		Super::Jump();
}

#pragma endregion

#pragma region Dash

void AMeteorShower_GP3Character::CancelDash()
{
	if(TimerManager->TimerExists(DashTimerHandle))
	{
		TimerManager->ClearTimer(DashTimerHandle);
		OnDashEnd();
	}
}

void AMeteorShower_GP3Character::Dash()
{
	if (!bCanDash || !bHasDash  || GetWorldTimerManager().TimerExists(DashCooldownTimerHandle) || GetWorldTimerManager().TimerExists(DashTimerHandle))
	{
		return;
	}
	
	OnDashStart();
	FTimerDelegate TimerDelegate;
	const FVector StartLocation = GetActorLocation();
	const FVector EndLocation =  GetPredictedDashDestination();
	TimerDelegate.BindUFunction(this,"TDashTimerTick",StartLocation,EndLocation);
	GetWorldTimerManager().SetTimer(DashTimerHandle,TimerDelegate,GetWorld()->DeltaTimeSeconds,true,-1);
}

void AMeteorShower_GP3Character::ResetDash()
{
	TimerManager->ClearTimer(DashCooldownTimerHandle);
}

void AMeteorShower_GP3Character::TDashTimerTick(const FVector StartPosition, const FVector PredictedPosition)
{
	DashTimer_Internal += GetWorldTimerManager().GetTimerElapsed(DashTimerHandle);
	//This is scaled to how close you are to the target, so that you're always moving with the same speed
	const float ScaledDashTime = DashSpeed * ((PredictedPosition - StartPosition).Length()/DashLength);
	const float EasedDashTimer = (DashEasingCurve != nullptr)? DashEasingCurve->GetFloatValue(DashTimer_Internal/ScaledDashTime):DashTimer_Internal/ScaledDashTime;
	FHitResult HitResult;
	SetActorLocation(StartPosition + (PredictedPosition-StartPosition) * EasedDashTimer,true,&HitResult,ETeleportType::None);
	if(HitResult.bBlockingHit || (DashTimer_Internal > ScaledDashTime))
	{
		OnDashEnd();
	}
}
float AMeteorShower_GP3Character::GetDashCooldownElapsed()
{
	if(!GetWorldTimerManager().TimerExists(DashCooldownTimerHandle))
	{
		return DashCooldown;
	}
	return GetWorldTimerManager().GetTimerElapsed(DashCooldownTimerHandle);
}
float AMeteorShower_GP3Character::GetDashCooldown()
{
	return DashCooldown;
}
void AMeteorShower_GP3Character::OnDashStart()
{
	if(Settings->DashSoundCue)
	{
		USoundHelper::PlaySoundSFX(this, Settings->DashSoundCue);
	}
	
	DashTimer_Internal = 0;
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	OnDashStart_Implementable();
	GetCapsuleComponent()->SetCapsuleSize(CapsuleWidth,10);
}

void AMeteorShower_GP3Character::OnDashEnd()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	GetCharacterMovement()->Launch(GetCharacterMovement()->Velocity);
	GetWorldTimerManager().ClearTimer(DashTimerHandle);
	GetWorldTimerManager().SetTimer(DashCooldownTimerHandle, GetWorld()->DeltaTimeSeconds, false, DashCooldown);
	OnDashEnd_Implementable();
	GetCapsuleComponent()->SetCapsuleSize(CapsuleWidth,CapsuleHeight);
}

FVector AMeteorShower_GP3Character::GetPredictedDashDestination()
{
	const FVector AlignedDashVectorLocation = GetActorLocation() +  GetAlignedDashVector()  * DashLength;
	FHitResult HitResult;
	const float TraceRadius = 12.0f;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	ActorsToIgnore.Add(this);

	//Check to see if the user is looking at a platform and if that platform is within reach
	bool HitObject = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		GetFirstPersonCameraComponent()->GetComponentLocation(),
		GetFirstPersonCameraComponent()->GetComponentLocation() + GetAlignedDashVector() * DashLength,
		TraceRadius,
		UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
		false,
		ActorsToIgnore,
		(EnableDashDebugLines)?EDrawDebugTrace::ForDuration: EDrawDebugTrace::None,
		HitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red,
		5.0f
	);

	if (HitObject && FVector::UpVector.Dot(HitResult.Normal) > 0.2f)
	{
		return HitResult.Location + CapsuleHeight * FVector::UpVector;
	}
	else if(HitObject)
	{
		return AlignedDashVectorLocation;
	}
	//If we didn't hit a platform then we trace downwards to see if we were just a little bit off,
	//We do this to help the user and make them less likely to hit an edge
	HitObject = UKismetSystemLibrary::SphereTraceSingle(
	GetWorld(),
	GetFirstPersonCameraComponent()->GetComponentLocation() + GetAlignedDashVector()  * DashLength,
	GetFirstPersonCameraComponent()->GetComponentLocation() + GetAlignedDashVector()  * DashLength +
	FVector::DownVector * AllowedHeightMargin,
	TraceRadius,
	UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
	false,
	ActorsToIgnore,
	(EnableDashDebugLines)?EDrawDebugTrace::ForDuration: EDrawDebugTrace::None,
	HitResult,
	true,
	FLinearColor::Green,
	FLinearColor::Red,
	5.0f
	 );
	if(HitObject)
	{
		return HitResult.Location + CapsuleHeight * FVector::UpVector;
	}

	//Check if the user would hit a platform if they'd dash in that direction;
	if(!HitObject)
	{
		HitObject = UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(),
			GetActorLocation() - FVector::UpVector * CapsuleHeight_WithoutHemisphere,
			GetActorLocation() - FVector::UpVector * CapsuleHeight_WithoutHemisphere + GetAlignedDashVector()  * DashLength,
			TraceRadius,
			UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
			false,
			ActorsToIgnore,
			(EnableDashDebugLines)?EDrawDebugTrace::ForDuration: EDrawDebugTrace::None,
			HitResult,
			true,
			FLinearColor::Green,
			FLinearColor::Red,
			5.0f);
		if(HitObject)
		{
			FHitResult LedgeHitResult;
			bool HitFloor = UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(),
			HitResult.Location + FVector::UpVector * 300 +  GetAlignedDashVector() * DashToLedgeMargin,
			HitResult.Location +  GetActorForwardVector() * DashToLedgeMargin,
			TraceRadius,
			UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
			false,
			ActorsToIgnore,
			(EnableDashDebugLines)?EDrawDebugTrace::ForDuration: EDrawDebugTrace::None,
		   LedgeHitResult,
		   true,
		   FLinearColor::Green,
		   FLinearColor::Red,
		   5.0f);
			if(HitFloor && FVector::UpVector.Dot(LedgeHitResult.Normal) > 0.2)
			{
				OnPredictedEdgeLand();
				return LedgeHitResult.Location + FVector::UpVector * CapsuleHeight;
			}
		}
	}
	
	return AlignedDashVectorLocation;
}

FVector AMeteorShower_GP3Character::GetAlignedDashVector()
{
	const FVector CameraForward = Cast<APlayerController>(GetController())->PlayerCameraManager->GetCameraRotation().
	                                                                        Vector().GetSafeNormal();
	if (GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		return CameraForward;
	}
	if (GetCharacterMovement()->MovementMode == MOVE_Walking)
	{
		FHitResult GroundHit = GetGroundHit();
		const FVector GroundForward = GetGroundForward();
		const float LookDot = FVector::UpVector.Dot(CameraForward);
		const float GroundForwardDot = FVector::UpVector.Dot(GroundForward);
		const float LookDirectionToGroundNormalDot = GroundHit.Normal.Dot(CameraForward);

		//if standing on an upwards slope and looking over the slope angle dash to where you're looking
		if (GroundForwardDot > 0 && LookDirectionToGroundNormalDot > 0)
		{
			return CameraForward;
		}
		//if Standing on an upwards slope and looking into the slope, dash along the slope
		if (GroundForwardDot > 0 && LookDirectionToGroundNormalDot < 0)
		{
			return GroundForward;
		}
		//if standing on a downwards slope and looking into the slope dash along the slope
		if (GroundForwardDot < 0 && LookDirectionToGroundNormalDot < 0)
		{
			return GroundForward;
		}
		//if stadning on a downwards slope and looking over the slope, dash to where youre looking
		if (GroundForwardDot < 0 && LookDirectionToGroundNormalDot > 0)
		{
			return CameraForward;
		}
		if (LookDot < 0)
		{
			return GroundForward;
		}
	}
	return CameraForward;
}

#pragma endregion
void AMeteorShower_GP3Character::Tick(float DeltaTime)
{
	if (DrawEdgeGrabDebugLines)
	{
		FWallClimbParams params;
		CanEdgeGrab(params);
	}
}

void AMeteorShower_GP3Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AMeteorShower_GP3Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		float MouseSens = 1.f;
		if(GameSettings)
		{
			MouseSens = GameSettings->MouseSensitivity;
		}
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X *  MouseSens);
		AddControllerPitchInput(LookAxisVector.Y * MouseSens);
	}
}

void AMeteorShower_GP3Character::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AMeteorShower_GP3Character::GetHasRifle()
{
	return bHasRifle;
}

void AMeteorShower_GP3Character::SetInputEnabled(bool Enabled)
{
	// auto PlayerController = GetWorld()->GetFirstPlayerController();
	// if (PlayerController)
	// {
		// if (Enabled)
		// {
			// PlayerController->SetInputMode(FInputModeGameOnly());
		// }
		// else
		// {
			// PlayerController->SetInputMode(FInputModeUIOnly());
		// }
	// }
	if (Enabled)
	{
		EnableInput(Cast<APlayerController>(GetController()));
	}
	else
	{
		DisableInput(Cast<APlayerController>(GetController()));
	}
}

FHitResult AMeteorShower_GP3Character::GetGroundHit()
{
	FHitResult GroundHit;

	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = TraceStart + FVector::UpVector * -300;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(GroundHit, TraceStart, TraceEnd, ECC_WorldDynamic, QueryParams);
	return GroundHit;
}


FVector AMeteorShower_GP3Character::GetGroundForward()
{
	if (GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Walking) { return GetActorForwardVector(); }

	FHitResult GroundHit = GetGroundHit();

	if (!GroundHit.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ground hit registered, either check the trace channel or length of trace"));
		return GetActorForwardVector();
	}

	FVector Forward = GetActorForwardVector();
	FVector PerVec = Forward.Cross(GroundHit.Normal).GetSafeNormal();
	FVector GroundAlignedForward = PerVec.Cross(GroundHit.Normal).GetSafeNormal();
	return -GroundAlignedForward;
}
