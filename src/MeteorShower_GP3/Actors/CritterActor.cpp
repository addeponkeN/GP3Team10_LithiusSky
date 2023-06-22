#include "CritterActor.h"

#include "Kismet/KismetSystemLibrary.h"
#include "MeteorShower_GP3/MeteorShower_GP3.h"
#include "MeteorShower_GP3/MeteorShower_GP3Character.h"

ACritterActor::ACritterActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACritterActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACritterActor::AttackForward()
{
	//	ATTACKED FORWARD
	auto Fw = GetActorForwardVector();
	auto FwUp = (Fw + FVector(0.f, 0.f, 1.f)).Normalize();
	auto Start = GetActorLocation() + Fw * 10.f;
	auto End = Start + FwUp * 100.f;
	AttackTowards(End);
}

void ACritterActor::AttackTowards(FVector TargetLocation)
{
	auto World = GetWorld();

	FVector Direction = TargetLocation - GetActorLocation();
	Direction.Normalize();

	constexpr float AttackRange = 120.f;

	FVector Start = GetActorLocation() + Direction * 10.f;
	FVector End = Start + Direction * AttackRange;

	FHitResult Result;

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		World,
		Start, End,
		25.f,
		ObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::ForDuration,
		Result,
		true);

	if (bHit)
	{
		AActor* HitActor = Result.GetActor();
		if (HitActor)
		{
			if (AMeteorShower_GP3Character* Character = Cast<AMeteorShower_GP3Character>(HitActor))
			{
				LOG("TUWTLE BITES!! AOWIEE!! ! 11!");
				Character->HealthComponent->Kill();
			}
		}
	}
}
