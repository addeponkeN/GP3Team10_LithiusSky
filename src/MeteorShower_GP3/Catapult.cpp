// Fill out your copyright notice in the Description page of Project Settings.


#include "Catapult.h"

#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACatapult::ACatapult()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CatapultMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
}
void ACatapult::Interact()
{
	if(!SpawnedRock || SkeletalMesh->IsPlaying() ||GetWorldTimerManager().TimerExists(RockFlyingTimerHandle) )return;
	TriggerSwingAnimation();
	
}
void ACatapult::RespawnRock()
{
	if(SpawnedRock)
	{
		SpawnedRock->Respawn(*SkeletalMesh,"SpoonSocket");
		SpawnedRock->RootMesh->SetSimulatePhysics(false);
		SpawnedRock->RootMesh->SetAllPhysicsLinearVelocity(FVector::Zero());
	}
}

FVector ACatapult::GetLaunchVector()
{
	const FVector SocketForward = SkeletalMesh->GetSocketRotation("SpoonSocket").Vector();
	const FVector SocketUp = UKismetMathLibrary::GetRightVector( SkeletalMesh->GetSocketRotation("SpoonSocket"));
	return SocketForward + (SocketUp * LaunchHeight);
}
void ACatapult::LaunchRock()
{
	GetWorldTimerManager().ClearTimer(RockFlyingTimerHandle);
	GetWorldTimerManager().SetTimer(RockFlyingTimerHandle,this, &ACatapult::RespawnRock, GetWorld()->DeltaTimeSeconds,false,RockRespawnTime);
	SpawnedRock->RootMesh->SetSimulatePhysics(true);
	SpawnedRock->SetActorEnableCollision(true);
	SpawnedRock->RootMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	FVector LaunchDirection = GetLaunchVector();
	//SpawnedRock->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SpawnedRock->RootMesh->AddImpulse(LaunchDirection * ImpulseForce);
}

// Called when the game starts or when spawned
void ACatapult::BeginPlay()
{
	Super::BeginPlay();
	SpawnedRock = GetWorld()->SpawnActor<ACatapult_Rock>(CatapultRock);
	SpawnedRock->Initialize();
	SetUpRock();
}


void ACatapult::SetUpRock()
{
	/*bool SocketExists = SkeletalMesh->DoesSocketExist("SpoonSocket");
	UE_LOG(LogTemp,Warning, TEXT("%s"), *((SocketExists)? FString("Socket Exists"): FString("Socket does not exist")));*/
	SpawnedRock->RootMesh->SetSimulatePhysics(false);
	SpawnedRock->SetActorEnableCollision(false);
	SpawnedRock->CoupleToSocket(SkeletalMesh,"SpoonSocket");
}

// Called every frame
void ACatapult::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void ACatapult::TriggerSwingAnimation()
{
	if(SkeletalMesh && AnimMontage)
	{
		SkeletalMesh->PlayAnimation(AnimMontage,false);
	}
}



