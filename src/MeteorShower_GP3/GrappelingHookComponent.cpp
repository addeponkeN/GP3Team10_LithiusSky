// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappelingHookComponent.h"
#include "MeteorShower_GP3Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GrappleInteractable.h"
#include "MeteorShower_GP3.h"
#include "SoundHelper.h"
#include "Components/AudioComponent.h"
#include "Evaluation/IMovieSceneEvaluationHook.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UGrappelingHookComponent::UGrappelingHookComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	const wchar_t* DataAssetPath = TEXT("/Game/Settings/DA_GrappleSettings.DA_GrappleSettings");
	static ConstructorHelpers::FObjectFinder<UGrappleSettings>GrappleSettings(DataAssetPath);
	
	if (GrappleSettings.Succeeded())
	{
		Settings = GrappleSettings.Object;
	}
	else
	{
		LOGWF("Could not find Character Settings Data Asset");
	}
}

// Called when the game starts
void UGrappelingHookComponent::BeginPlay()
{
	Super::BeginPlay();
	QueryForGrapplingHookMesh();
	
	Cable = NewObject<UCableComponent>(this, TEXT("GrapplingCable"));
	Cable->NumSegments = 1;
	Cable->CableWidth = 4;
	Cable->RegisterComponent();
	Cable->AttachToComponent(GrapplingGunMesh,FAttachmentTransformRules::SnapToTargetNotIncludingScale,FireLocationSocketName);
	Cable->TileMaterial = Settings->MaterialTiling;
	Cable->SetMaterial(0,Settings->RopeMaterial);
	if(IsValid(GetWorld()))
	{
		TimerManager = &GetWorld()->GetTimerManager();
	}
	if(!HookObject)return;
	CableAttachActor = GetWorld()->SpawnActor<AGrapplingHookObject>(HookObject,FVector::Zero(),FRotator::ZeroRotator);
	SetUpCable();
}
void UGrappelingHookComponent::OnGrappleStart_Internal()
{
	if(Settings->Hit)
	{
		USoundHelper::PlaySoundSFX3D(this, Settings->Hit, AttachedPoint);
	}
	if(Settings->Reel)
	{
		ReelAudioComponent = USoundHelper::PlaySoundSFX(this, Settings->Reel);
	}
	OnGrappleStart();
	Character->OnGrappleStart();
}
void UGrappelingHookComponent::OnGrappleEnd_Internal()
{
	if(ReelAudioComponent)
	{
		ReelAudioComponent->Stop();
		ReelAudioComponent = nullptr;
	}
	OnGrappleEnd();
	Cable->SetHiddenInGame(true);
	Character->OnGrappleEnd();
}

#pragma region CableInternal

void UGrappelingHookComponent::SetUpCable()
{
	if(!Cable){UE_LOG(LogTemp,Warning, TEXT("No Cable Found in Set up"))return;}
	Cable->SetHiddenInGame(true);
	if(CableAttachActor != nullptr)
	{
		const bool DoesSocketExist = CableAttachActor->GetStaticMesh()->DoesSocketExist(RopeAttachSocket);
		Cable->SetAttachEndTo(CableAttachActor,FName(CableAttachActor->GetStaticMesh()->GetName()), RopeAttachSocket);
		Cable->EndLocation = FVector(0,0,0);
	}
	else
	{
		UE_LOG(LogTemp,Warning, TEXT("Can't find CableAttachActor on setup"));
	}
}
void UGrappelingHookComponent::UpdateCableEndLocation(FVector AttachPoint)
{
	CableAttachActor->SetActorLocation(AttachPoint);
}
void UGrappelingHookComponent::StartCableHookAnimation()
{
	HookAnimationTimer = 0;
	
	HookStartPosition = GetStartAttachPoint();
	
	HookPath = AttachedPoint - HookStartPosition;
	
	AnimatingHook = true;
	
	CableAttachActor->GetStaticMesh()->SetWorldRotation(HookPath.GetSafeNormal().ToOrientationRotator());
	Cable->SetHiddenInGame(false);
	CableAttachActor->SetActorHiddenInGame(false);

	AdjustedHookAnimationTime = HookAnimationTime * (FVector::Distance(GetStartAttachPoint(),AttachedPoint)/MaxHitRange);
	if(TimerManager)
	{
		    TimerManager->SetTimer(HookAnimationTimerHandle,
							this,
							&UGrappelingHookComponent::TAnimateCableHookAbject,
							GetWorld()->DeltaTimeSeconds,
							true,
							-1);
		
	}
}

void UGrappelingHookComponent::TAnimateCableHookAbject()
{
	
	HookAnimationTimer += GetWorld()->DeltaTimeSeconds;
	
	const float EasedProcentualPathValue = (HookAnimationCurve != nullptr)?
		HookAnimationCurve->GetFloatValue(HookAnimationTimer/AdjustedHookAnimationTime):
	HookAnimationTimer/AdjustedHookAnimationTime
	;
	
	CableAttachActor->SetActorLocation(HookStartPosition + EasedProcentualPathValue * HookPath);
	
	if(HookAnimationTimer >= AdjustedHookAnimationTime)
	{
		AttachGrapplePoint(AttachedPoint);
		
		AnimatingHook = false;
		
		TimerManager->ClearTimer(HookAnimationTimerHandle);
		
	}
}
#pragma endregion

bool UGrappelingHookComponent::ShouldDeattach()
{
	if(AttachedPoint == FVector::Zero())
	{
		return true;
	}
	//Read the object that the hook is attached to 
	FHitResult Hit;
	FVector CameraForward = PlayerController->PlayerCameraManager->GetCameraRotation().Vector();
	FVector TraceStart = GetStartAttachPoint();
	FVector TraceEnd = AttachedPoint + (TraceStart - AttachedPoint).GetSafeNormal();
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.AddIgnoredActor(Character);

	GetWorld()->LineTraceSingleByChannel(Hit,TraceStart,TraceEnd, DetachTraceChannelProperty, QueryParams);
	if(Hit.bBlockingHit)
	{
		LOGW("Detach: Something in the way");
		return true;
	}
	
	FVector AttachPointToCharacterDirection = (Character->GetActorLocation() - AttachedPoint).GetSafeNormal();
	
	float DotProduct = AttachPointToCharacterDirection.Dot(AttachedNormal);
	
	float DistanceToAttachedPoint = (Character->GetActorLocation() - AttachedPoint).Length();

	FVector RopeDirection = (TraceEnd - TraceStart).GetSafeNormal();
	
	FVector LookDirection = PlayerController->PlayerCameraManager->GetCameraRotation().Vector().GetSafeNormal();
	
	float RopeLookDot = RopeDirection.Dot(LookDirection);

	if(DotProduct < DeatachAngleMargin)
	{
		LOGW("Detach: Angle too Obtuse");
	}
	if(DistanceToAttachedPoint < DeatachDistance )
	{
		LOGW("Detach: Reached deatach distance");
	}
	if(RopeLookDot < LookAwayDetachMargin)
	{
		LOGW("Detach: Looking away from attached point");
	}
	
	return DotProduct < DeatachAngleMargin || DistanceToAttachedPoint < DeatachDistance || RopeLookDot < LookAwayDetachMargin; 
}

bool UGrappelingHookComponent::ValidateFireLocation(FHitResult &Out)
{
	
	FHitResult Hit;
	FVector CameraForward = PlayerController->PlayerCameraManager->GetCameraRotation().Vector();
	FVector TraceStart = GetStartAttachPoint();
	FVector TraceEnd = TraceStart + CameraForward * MaxHitRange;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.AddIgnoredActor(Character);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	ActorsToIgnore.Add(Character);
	
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(TraceChannelProperty);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectQueryArray;
	ObjectQueryArray.Add(UEngineTypes::ConvertToObjectType(TraceChannelProperty));

	bool HitGrappable = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		TraceStart,
		TraceEnd,
		SphereTraceRadius,
		ObjectQueryArray,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		Hit,
		true,
		FColor::Green,
		FColor::Red,
		10
	);
	
	//GetWorld()->LineTraceSingleByObjectType(Hit,TraceStart,TraceEnd,ObjectParams , QueryParams);

	if(Hit.bBlockingHit)
	{
		//DrawDebugLine(GetWorld(),TraceStart,Hit.Location,FColor::Green,false,2.0f,0,10.f);
		//DrawDebugSphere(GetWorld(),Hit.Location,30.0f,16.0f,FColor::Green,false,2.0f,0,10.0f);
		Out = Hit;
		AttachedNormal = Hit.Normal;
		//UE_LOG(LogTemp,Warning,TEXT("%s"), *Hit.Location.ToString());
		return true;
	}
	
	//DrawDebugLine(GetWorld(),TraceStart,TraceEnd, FColor::Red,false,2.0f,0,10.f);		
	return false;
}

FVector UGrappelingHookComponent::GetStartAttachPoint()
{
	if(!GrapplingGunMesh)
	{
		QueryForGrapplingHookMesh();
	}
	if(GrapplingGunMesh->DoesSocketExist(FireLocationSocketName))
	{
		return GrapplingGunMesh->GetSocketTransform(FireLocationSocketName).GetLocation();
	}
	return GrapplingGunMesh->GetComponentLocation();
}

void UGrappelingHookComponent::AttachGrapplePoint(FVector &AttachPoint)
{
	if(TimerManager != nullptr && !TimerManager->TimerExists(AttachGrappleHandle))
	{
		OnGrappleStart_Internal();
		bInUse = true;
		//TimerManager->ClearTimer(AttachGrappleHandle);
		FTimerDelegate Delegate;
		Delegate.BindUObject(this,&UGrappelingHookComponent::THookAttackedTick,AttachPoint, Character->GetCharacterMovement()->Velocity);
		TimerManager->SetTimer(AttachGrappleHandle,Delegate, GetWorld()->GetDeltaSeconds(),true,-1.f);
		TimeSinceAttached = 0;
		UpdateCableEndLocation(AttachPoint);
	}
}
void UGrappelingHookComponent::HandleDeattach()
{
	TriggerReelInAnimation();
	OnGrappleEnd_Internal();
	Character->GetMesh()->SetEnableGravity(true);
	TimerManager->ClearTimer(AttachGrappleHandle);
}

void UGrappelingHookComponent::THookAttackedTick(FVector AttachPoint,FVector StartVelocity)
{
	UpdateCableEndLocation(AttachPoint);
	UCharacterMovementComponent* charMovement = Character->GetCharacterMovement();
	Character->GetMesh()->SetEnableGravity(false);

	float reelInEased = ReelInStrength;
	if(reelInEasing != nullptr)
	{
		reelInEased = StartVelocity.Length() + (ReelInStrength - StartVelocity.Length()) * reelInEasing->GetFloatValue(FMath::Clamp((TimeBeforeTerminalVelocity == 0)? 1: TimeSinceAttached/TimeBeforeTerminalVelocity,0,1));
	}
	charMovement->Launch(GetGrappleTravelDirection() * reelInEased);
	
	
	if(ShouldDeattach())
	{
		HandleDeattach();
	}
	TimeSinceAttached += GetWorld()->GetDeltaSeconds();
}

float UGrappelingHookComponent::GetReverseMultiplier(FVector DesiredDirection, FVector CurrentVelocity)
{
	float Dot = CurrentVelocity.GetSafeNormal().Dot(DesiredDirection.GetSafeNormal());
	float ReverseMultiplier = ((1 - Dot)/2) * ReverseAccelerationMultiplier;
	return ReverseMultiplier;
}

void UGrappelingHookComponent::ToggleHook()
{
	if(AnimatingHook || ShootAnimInProgress)return;
	if(bInUse)
	{
		HandleDeattach();
	}
	else
	{
		Fire();
	}
}

void UGrappelingHookComponent::Fire()
{
	FHitResult HitResult;
	if(ValidateFireLocation(HitResult))
	{
		if(IGrappleInteractable* GrappleInteractable = Cast<IGrappleInteractable>(HitResult.GetActor()))
		{
			GrappleInteractable->Interact();
			return;
		}
		AttachedPoint = HitResult.ImpactPoint;
		TriggerShootAnimation();
	}
}

#pragma region Animations
void UGrappelingHookComponent::TriggerShootAnimation()
{
	if(ShootAnimInProgress){return;}
	if(Settings->Shoot_Anim)
	{
		GrapplingGunMesh->PlayAnimation(Settings->Shoot_Anim,false);
		ShootAnimInProgress = true;
	}
	else
	{
		OnShootFinished();
	}
}
void UGrappelingHookComponent::OnShootFinished()
{
	if(Settings->Shoot)
	{
		USoundHelper::PlaySoundSFX(this, Settings->Shoot);
	}
	ShootAnimInProgress = false;
	StartCableHookAnimation();
}
void UGrappelingHookComponent::TriggerReelInAnimation()
{
	if(Settings->ReelIn_Anim)
	{
		GrapplingGunMesh->PlayAnimation(Settings->ReelIn_Anim,false);
	}
	else
	{
		OnReelInAnimationFinished();
	}
	ShootAnimInProgress = true;
}
void UGrappelingHookComponent::OnReelInAnimationFinished()
{
	if(Settings->Idle_Anim)
	{
		GrapplingGunMesh->PlayAnimation(Settings->Idle_Anim,true);
	}
	bInUse = ShootAnimInProgress = false;
}



#pragma endregion 


// Called every frame
void UGrappelingHookComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}
void UGrappelingHookComponent::AttachHook(AMeteorShower_GP3Character* TargetCharacter)
{
	Character = TargetCharacter;
	if(Character == nullptr)
	{
		return;
	}
	PlayerController = Cast<APlayerController>(Character->GetController());
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
    AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	Character->bHasGrapplingHook = true;
	Character->GrapplingHook = this;
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, & UGrappelingHookComponent::ToggleHook);
		}
	}
	AttachGrapplingMeshToTimeGunMesh(TargetCharacter);
	StartProcessingOnLookEvents();
}
void UGrappelingHookComponent::QueryForGrapplingHookMesh()
{	
	TArray<USkeletalMeshComponent*> MeshComponents;
	GetOwner()->GetComponents<USkeletalMeshComponent>(MeshComponents);

	for(USkeletalMeshComponent* sm: MeshComponents)
	{
		if(sm->GetName() == GrapplingGunMeshName)
		{
			GrapplingGunMesh = sm;
		}
	}
}
void UGrappelingHookComponent::AttachGrapplingMeshToTimeGunMesh(AMeteorShower_GP3Character* TargetCharacter)
{
	TArray<UStaticMeshComponent*> MeshComponents;
	TargetCharacter->GetComponents<UStaticMeshComponent>(MeshComponents);
	for(UStaticMeshComponent* sm : MeshComponents)
	{
		if(sm->GetName() == TimeGunMeshName)
		{
			if(!GrapplingGunMesh)
			{
				QueryForGrapplingHookMesh();
			}
			if(!GrapplingGunMesh){return;}
			GrapplingGunMesh->AttachToComponent(sm,FAttachmentTransformRules::SnapToTargetNotIncludingScale,GrapplingHookAttachSocketName);
			bHasBeenPickedUp = true;
		}
	}
	GrapplingGunMesh->SetRelativeRotation(FRotator(0,90,0));
	GrapplingGunMesh->SetWorldScale3D(SizeOnAttach);
}

FVector UGrappelingHookComponent::GetGrappleTravelDirection()
{
	const FVector PlayerToAttachPoint = (AttachedPoint - Character->GetActorLocation()).GetSafeNormal();
	const FVector CameraLookDirection = PlayerController->PlayerCameraManager->GetCameraRotation().Vector().GetSafeNormal();
	const FVector TravelDirection = (PlayerToAttachPoint+CameraLookDirection).GetSafeNormal();
#pragma region
	//DrawDebugLine(GetWorld(),Character->GetActorLocation(),Character->GetActorLocation() + ( TravelDirection* 1000), FColor::Magenta,false,GetWorld()->GetDeltaSeconds(),0,10.0f);
#pragma endregion 
	return TravelDirection;
}
#pragma region OnLookEvents
void UGrappelingHookComponent::TProcessOnLookEvents()
{
	FHitResult Hit;
	if(ValidateFireLocation(Hit))
	{
		UGrappableOverlay* GrappableOverlay = Cast<UGrappableOverlay>(Hit.GetActor()->GetComponentByClass(UGrappableOverlay::StaticClass()));
		if(GrappableOverlay)
		{
			if(GrappableOverlay != CurrentOnLookTarget)
			{
				ClearOnLookTarget();
				CurrentOnLookTarget = GrappableOverlay;
				CurrentOnLookTarget->OnLookBegin();
				Character->OnLookBegin.Broadcast();
			}
		}
	}
	else
	{
		ClearOnLookTarget();
	}
}
void UGrappelingHookComponent::ClearOnLookTarget()
{
	if(CurrentOnLookTarget == nullptr){return;}
	Character->OnLookEnd.Broadcast();
	CurrentOnLookTarget->OnLookEnd();
	CurrentOnLookTarget = nullptr;
}
void UGrappelingHookComponent::StartProcessingOnLookEvents()
{
	TimerManager->ClearTimer(ProcessOnLookEventsTimerHandle);
	if(TimerManager)
	{
		TimerManager->SetTimer(ProcessOnLookEventsTimerHandle,this,&UGrappelingHookComponent::TProcessOnLookEvents,OnLookEventsTickRate,true,false);
	}
}


#pragma endregion 