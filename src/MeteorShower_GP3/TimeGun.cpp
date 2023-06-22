#include "TimeGun.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "MeteorShower_GP3.h"
#include "MeteorShower_GP3Character.h"
#include "SoundHelper.h"
#include "TimeGunLaser.h"
#include "TimeStoppable.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

UTimeGun::UTimeGun()
{
	PrimaryComponentTick.bCanEverTick = false;

	const wchar_t* TimeGunDataAssetPath = TEXT("/Game/Settings/DA_TimeGun.DA_TimeGun");
	static ConstructorHelpers::FObjectFinder<UTimeGunDataAsset>
		TimeGunDataAsset(TimeGunDataAssetPath);
	if (TimeGunDataAsset.Succeeded())
	{
		Settings = TimeGunDataAsset.Object;
	}
	else
	{
		LOGWF("Could not find TimeGun Data Asset '%s'", TimeGunDataAssetPath);
	}
}

void UTimeGun::BeginPlay()
{
	Super::BeginPlay();

	TArray<USceneComponent*> Children;
	GetChildrenComponents(false, Children);

	for (USceneComponent* const Child : Children)
	{
		if (UArrowComponent* MuzzleArrow = Cast<UArrowComponent>(Child))
		{
			MuzzleLocation = MuzzleArrow;
			break;
		}
	}

	if (!MuzzleLocation)
	{
		const FName MuzzleSocketName = FName(TEXT("Muzzle"));
		const FAttachmentTransformRules AttachRule = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
		MuzzleLocation = NewObject<UArrowComponent>(this, FName(TEXT("AutoMuzzleLocation")));
		MuzzleLocation->RegisterComponent();
		MuzzleLocation->AttachToComponent(this, AttachRule, MuzzleSocketName);
		LOGW("Added default muzzle location to TimeGun");
	}

	// MuzzleLocation = CreateDefaultSubobject<UArrowComponent>(FName(TEXT("MuzzleLocation")));

	if (Settings)
	{
		AMeteorShower_GP3Character* CharacterToAttach = Cast<AMeteorShower_GP3Character>(GetOwner());
		if (CharacterToAttach)
		{
			AttachToCharacter(CharacterToAttach);
		}
		if (!Settings->BeamEffectClass)
		{
			Settings->BeamEffectClass = ATimeGunLaser::StaticClass();
			LOGW("TimeGun missing beam effect");
		}
	}
	else
	{
		LOGE("Could not attach TimeGun!! UTimeGun::BeginPlay() > AttachToCharacter()");
	}


	StartLineTraceTimeStoppableObjects();
}

void UTimeGun::OnComponentCreated()
{
	Super::OnComponentCreated();
}

void UTimeGun::StartLineTraceTimeStoppableObjects()
{
	if (!Settings)
	{
		LOGE("!! TimeGun missing DataAsset !!");
		return;
	}
	GetWorld()->GetTimerManager()
	          .SetTimer(TracerTimerHandle, this, &UTimeGun::LineTraceStoppableObjects,
	                    Settings->Stats.LineTraceIntervalSeconds, true);
}

void UTimeGun::LineTraceStoppableObjects()
{
	if (!Character)
	{
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());

	if (!PlayerController)
	{
		return;
	}
	TObjectPtr<APlayerCameraManager> Camera = PlayerController->PlayerCameraManager;

	const FVector LookDirection = Camera->GetActorForwardVector();
	const FVector SpawnLocation = Camera->GetCameraLocation() + LookDirection * 50.f;
	const FVector EndLocation = SpawnLocation + LookDirection * Settings->Stats.ShootRange;

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(Settings->CollisionChannel);
	FHitResult Result;
	if (!GetWorld()->LineTraceSingleByObjectType(Result, SpawnLocation, EndLocation, ObjParams))
	{
		ClearLookTarget();
		return;
	}

	AActor* HitActor = Result.GetActor();

	if (!HitActor)
	{
		ClearLookTarget();
		return;
	}

	UTimeStoppable* StoppableComponent =
		Cast<UTimeStoppable>(HitActor->GetComponentByClass(UTimeStoppable::StaticClass()));

	if (!StoppableComponent)
	{
		ClearLookTarget();
		return;
	}

	if (CurrentLookTarget == StoppableComponent)
	{
		return;
	}

	ClearLookTarget();

	CurrentLookTarget = StoppableComponent;
	CurrentLookTarget->OnLookBegin();
}

void UTimeGun::ClearLookTarget()
{
	if (CurrentLookTarget)
	{
		CurrentLookTarget->OnLookEnd();
		CurrentLookTarget = nullptr;
	}
}

void UTimeGun::Shoot()
{
	// LOG("PEW");
	UWorld* World = GetWorld();

	if (!World)
	{
		LOGE("World null on TimeGun::Shoot()");
		return;
	}

	if(!Character->InputEnabled())
	{
		return;
	}

	OnShootEvent.Broadcast();

	if (Settings->SFX.ShootSound)
	{
		USoundHelper::PlaySoundSFX(this, Settings->SFX.ShootSound);
		// UGameplayStatics::PlaySound2D(this, Settings->SFX.ShootSound);
	}

	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	TObjectPtr<APlayerCameraManager> Camera = PlayerController->PlayerCameraManager;

	const FVector LookDirection = Camera->GetActorForwardVector();
	const FVector StartLocation = Camera->GetCameraLocation() + LookDirection * 50.f;
	const FVector EndLocation = StartLocation + LookDirection * Settings->Stats.ShootRange;

	FHitResult Result;
	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(Settings->CollisionChannel);
	bool LineTraceHitAnything = true;
	bool LineTraceHitStoppable = World->LineTraceSingleByObjectType(Result, StartLocation, EndLocation, ObjParams);
	if (!LineTraceHitStoppable)
	{
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Character);
		LineTraceHitAnything = World->LineTraceSingleByChannel(Result, StartLocation, EndLocation, ECC_WorldStatic,
		                                                       Params);
	}

	FVector HitLocation = LineTraceHitAnything ? Result.Location : EndLocation;

	ATimeGunLaser* Laser = GetWorld()->SpawnActor<ATimeGunLaser>(
		Settings->BeamEffectClass, MuzzleLocation->GetComponentLocation(), FRotator::ZeroRotator);
	FName SocketName = FName("Muzzle");
	Laser->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	Laser->SetDestination(HitLocation);

	HandleHitResult(Result);

	// if (LineTraceHitStoppable && Settings->SFX.ShootHitTargetSound)
	// {
	// UGameplayStatics::PlaySound2D(this, Settings->SFX.ShootHitTargetSound);
	// }
}

bool UTimeGun::HandleHitResult(FHitResult& Result)
{
	AActor* HitActor = Result.GetActor();

	if (!HitActor)
	{
		// LOG("no actor hit");
		//	no actor hit, return
		return false;
	}

	// UStaticMeshComponent* HitMeshComponent = Cast<UStaticMeshComponent>(Result.GetComponent());
	// if (!HitMeshComponent)
	// {
	// LOG("no mesh hit");
	// return false;
	// }

	UTimeStoppable* StoppableComponent =
		Cast<UTimeStoppable>(HitActor->GetComponentByClass(UTimeStoppable::StaticClass()));

	if (!StoppableComponent)
	{
		// LOGWF("Hit actor was not Stoppable (%s)", *HitActor->GetName());
		return false;
	}

	if (CurrentStoppedObject && IsValid(CurrentStoppedObject))
	{
		if (CurrentStoppedObject == StoppableComponent)
		{
			// LOG("Tried to freeze the same target twice");
			StoppableComponent->StartUnFreeze(true);
			OnTimeUnstoppedTargetEvent.Broadcast(StoppableComponent);
			ClearFreezeTarget();
			return true;
		}
		CurrentStoppedObject->StartUnFreeze(false);
		// LOG("Unfreezed previous object");
	}

	FreezeTarget(StoppableComponent);

	OnTimeStoppedTargetEvent.Broadcast(StoppableComponent);

	return true;
}

void UTimeGun::AttachToCharacter(AMeteorShower_GP3Character* Target)
{
	Character = Target;

	//	todo: actual attachment
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(Settings->FireTimeGunMappingContext, 1);
		}
		UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
		if (InputComponent)
		{
			InputComponent->BindAction(Settings->FireTimeGunAction, ETriggerEvent::Triggered, this, &UTimeGun::Shoot);
		}
	}
}

void UTimeGun::FreezeTarget(UTimeStoppable* StoppableTarget)
{
	CurrentStoppedObject = StoppableTarget;
	CurrentStoppedObject->Freeze(Settings->Stats.SlowDuration, Settings->Stats.SlowAmount);
	GetWorld()->GetTimerManager().SetTimer(ClearCurrentStoppableObjectTimerHandle,
	                                       this, &UTimeGun::ClearFreezeTarget, Settings->Stats.SlowDuration);
}

void UTimeGun::ClearFreezeTarget()
{
	CurrentStoppedObject = nullptr;
}
