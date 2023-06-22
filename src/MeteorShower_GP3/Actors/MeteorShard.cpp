#include "MeteorShard.h"

AMeteorShard::AMeteorShard()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMeteorShard::BeginPlay()
{
	Super::BeginPlay();
	LifeTimer = ImpactTime;
}

void AMeteorShard::InitShard(FVector StartLocation, FVector EndLocation)
{
	Start = StartLocation;
	End = EndLocation;

	constexpr float MaxDistance = 15000.f;
	const float Distance = FVector::Distance(Start, End);

	// ImpactTime = (Distance / MaxDistance) / ImpactTime;
}

void AMeteorShard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LifeTimer -= DeltaTime;
	const float Alpha = 1.f - (LifeTimer / ImpactTime);
	UpdatePosition(Alpha);
	if (LifeTimer <= 0.f)
	{
		Impact();
	}
}

void AMeteorShard::UpdatePosition(float Alpha)
{
	const FVector Location = FMath::Lerp(Start, End, Alpha);
	SetActorLocation(Location);
}

void AMeteorShard::Impact()
{
	SetActorTickEnabled(false);
	auto Mesh = GetComponentByClass(UStaticMeshComponent::StaticClass());
	if (Mesh)
	{
		Mesh->DestroyComponent();
	}
	GetWorld()->GetTimerManager().SetTimer(LifeTimerHandle, this, &AMeteorShard::KillShard, 6.f);
	OnImpact.Broadcast();
}

void AMeteorShard::KillShard()
{
	Destroy();
}
