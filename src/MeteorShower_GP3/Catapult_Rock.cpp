// Fill out your copyright notice in the Description page of Project Settings.


#include "Catapult_Rock.h"

#include "Math/UnitConversion.h"

// Sets default values
ACatapult_Rock::ACatapult_Rock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACatapult_Rock::BeginPlay()
{
	Super::BeginPlay();
}
void ACatapult_Rock::Initialize()
{
	TArray<UStaticMeshComponent*> Components;
	GetComponents<UStaticMeshComponent>(Components);
	RootMesh = Components[0];
	RockMaterial = UMaterialInstanceDynamic::Create(RootMesh->GetMaterial(0),this);
	RootMesh->SetMaterial(0, RockMaterial);
	TimerManger = &GetWorld()->GetTimerManager();
}

// Called every frame
void ACatapult_Rock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void ACatapult_Rock::ResetRespawnTimer()
{
	TimerManger->ClearTimer(RespawnTimerHandle);
	RespawnSequenceState = ERespawnSequenceState::None;
	RespawnTimer = 0;
	RockMaterial->SetScalarParameterValue("CutOff", 0);
}
void ACatapult_Rock::TRespawnTick(USkeletalMeshComponent* SkeletalMeshComponent, FString SocketName)
{
	RespawnTimer += TimerManger->GetTimerElapsed(RespawnTimerHandle);
	switch(RespawnSequenceState)
	{
		case ERespawnSequenceState::None: break;
		case ERespawnSequenceState::Hiding:
			RockMaterial->SetScalarParameterValue("CutOff",RespawnTimer/DissolveTime);
			if(RespawnTimer > DissolveTime)
			{
				RespawnSequenceState = ERespawnSequenceState::Moving;
			}
		break;
	case ERespawnSequenceState::Moving:
			CoupleToSocket(SkeletalMeshComponent,SocketName);
			RespawnTimer = 0;
			RespawnSequenceState = ERespawnSequenceState::Showing;
			break;
		case ERespawnSequenceState::Showing:
			RockMaterial->SetScalarParameterValue("CutOff",1-(RespawnTimer/DissolveTime));
			if(RespawnTimer > DissolveTime)
			{
				ResetRespawnTimer();
			}
			break;
	}	
}
void ACatapult_Rock::CoupleToSocket(USkeletalMeshComponent* SkeletalMeshComponent, FString SocketName)
{
	RootMesh->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName(SocketName));
	RootMesh->SetRelativeLocation(FVector::Zero());
	RootMesh->SetRelativeRotation(SkeletalMeshComponent->GetSocketRotation(FName(SocketName)));
	UE_LOG(LogTemp, Warning,  TEXT("%s"), *((this->IsAttachedTo(SkeletalMeshComponent->GetOwner()))? FString("Attached To catapult"): FString("NOt Attached To catapult")));
}

void ACatapult_Rock::Respawn(USkeletalMeshComponent &SkeletalMeshComponent, FString SocketName)
{
	 if(!TimerManger)
	 {
		 TimerManger = &GetWorld()->GetTimerManager();
	 }
	TimerManger->ClearTimer(RespawnTimerHandle);
	RespawnSequenceState = ERespawnSequenceState::Hiding;
	SetActorEnableCollision(false);
	FTimerDelegate Delegate;
	Delegate.BindUObject(this,&ACatapult_Rock::TRespawnTick,&SkeletalMeshComponent,SocketName);
	TimerManger->SetTimer(RespawnTimerHandle, Delegate,GetWorld()->DeltaTimeSeconds,true,-1);
}


