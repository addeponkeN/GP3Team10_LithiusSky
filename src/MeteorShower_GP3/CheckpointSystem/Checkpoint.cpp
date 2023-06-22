#include "Checkpoint.h"

ACheckpoint::ACheckpoint()
{
	PrimaryActorTick.bCanEverTick = false;

	Area = CreateDefaultSubobject<UBoxComponent>(FName("CollideArea"));
	Area->SetCollisionProfileName("Checkpoint");
	RootComponent = Area;

	RespawnLocation = CreateDefaultSubobject<UStaticMeshComponent>(FName("RespawnLocation"));
	RespawnLocation->SetupAttachment(Area);

	ForwardArrow = CreateDefaultSubobject<UArrowComponent>(FName("LookDirection"));
	ForwardArrow->SetupAttachment(RespawnLocation);
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
}

void ACheckpoint::SetIsCurrent(bool IsCurrent)
{
	OnIsSetCurrent.Broadcast(IsCurrent);
}
