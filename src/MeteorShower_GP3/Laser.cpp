#include "Laser.h"

ALaser::ALaser()
{
	PrimaryActorTick.bCanEverTick = false;
	Arrow = CreateDefaultSubobject<UArrowComponent>(FName("ArrowComponent"));
	Arrow->SetHiddenInGame(false);
	RootComponent = Arrow;
}

void ALaser::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(KillTimerHandle, this, &ALaser::DestroyLaser, LifeTime, false);
}

void ALaser::SetDestination(FVector const& Destination)
{
	const FVector SelfLocation = GetActorLocation();

	const float DistanceToDestination = FVector::Distance(SelfLocation, Destination);

	FVector DirectionToDestination = Destination - SelfLocation;
	DirectionToDestination.Normalize();

	const auto RotationToDestination = FRotationMatrix::MakeFromX(DirectionToDestination).Rotator();

	Arrow->ArrowLength = DistanceToDestination;
	Arrow->SetWorldRotation(RotationToDestination);
	Arrow->SetArrowColor(FLinearColor::Blue);		// fully refreshes the arrow
}

void ALaser::DestroyLaser()
{
	Destroy();
}

void ALaser::SetLifeTime(float Time)
{
	LifeTime = Time;
	GetWorldTimerManager().ClearTimer(KillTimerHandle);
	GetWorldTimerManager().SetTimer(KillTimerHandle, this, &ALaser::DestroyLaser, LifeTime, false);
}
