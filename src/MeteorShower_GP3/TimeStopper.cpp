// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeStopper.h"
#include "MeteorShower_GP3.h"
#include "TimeStoppable.h"

UTimeStopper::UTimeStopper()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTimeStopper::BeginPlay()
{
	Super::BeginPlay();
	UPrimitiveComponent* collider = Cast<UPrimitiveComponent>(
		GetOwner()->GetComponentByClass(UPrimitiveComponent::StaticClass()));
	if (collider)
	{
		collider->OnComponentHit.AddDynamic(this, &UTimeStopper::OnHit);
	}
	else
	{
		FString className = GetOwner()->GetClass()->GetName();
		LOG(FString::Printf(TEXT("Failed to setup collider for UTimeStopper component in class '%s'"), *className));
	}
}

void UTimeStopper::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                         FVector NormalImpulse, const FHitResult& Hit)
{
	UTimeStoppable* stoppable = Cast<UTimeStoppable>(OtherComp);
	if (stoppable)
	{
		//	freeze for 2 seconds if colliding with a TimeStoppable object
		stoppable->Freeze(SlowDuration, SlowAmount);
	}
}
