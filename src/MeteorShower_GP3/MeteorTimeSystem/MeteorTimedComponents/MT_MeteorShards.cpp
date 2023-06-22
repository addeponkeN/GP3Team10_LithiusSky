#include "MT_MeteorShards.h"

#include "Kismet/KismetMathLibrary.h"
#include "MeteorShower_GP3/MSGameMode.h"
#include "MeteorShower_GP3/Actors/MeteorShard.h"
#include "MeteorShower_GP3/DataAssets/MeteorShardsDataSettings.h"

UMT_MeteorShards::UMT_MeteorShards()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMT_MeteorShards::BeginPlay()
{
	Super::BeginPlay();
	World = GetWorld();
	auto MeteorTimer = AMSGameMode::GetMeteorTimerStatic(this);
	if (MeteorTimer)
	{
		MeteorTimer->OnTimerStoppedEvent.AddDynamic(this, &UMT_MeteorShards::ClearAllShards);
	}
}

void UMT_MeteorShards::Register(AMeteorTimer* MeteorTimer)
{
	IMeteorTimed::Register(MeteorTimer);
	CharacterActor = World->GetFirstPlayerController()->GetPawn();
}

void UMT_MeteorShards::OnSetTime(AMeteorTimer* MeteorTimer)
{
	IMeteorTimed::OnSetTime(MeteorTimer);

	const float ImpactTime = MeteorTimer->GetTimer();

	if (ImpactTime < Settings->WhenToStart)
	{
		const float Alpha = MeteorTimer->GetPercent();
		const float SpawnInterval = FMath::Lerp(Settings->SpawnIntervalEnd, Settings->SpawnIntervalStart, Alpha);
		IntervalTimer += MeteorTimer->TimerTickRate;
		if (IntervalTimer >= SpawnInterval)
		{
			IntervalTimer -= SpawnInterval * FMath::RandRange(0.5f, 1.5f);
			SpawnMeteor();
		}
	}
}

void UMT_MeteorShards::ClearAllShards(AMeteorTimer* MeteorTimer)
{
	for (AActor* Shard : Shards)
	{
		if (Shard && IsValid(Shard))
		{
			Shard->Destroy();
		}
	}
	Shards.Empty();
}

void UMT_MeteorShards::OnShardDestroyed(AActor* DestroyedShard)
{
	DestroyedShard->OnDestroyed.RemoveAll(this);
	Shards.Remove(DestroyedShard);
}

void UMT_MeteorShards::SpawnMeteor()
{
	FVector MeteorLocation = GetRandomLocation();
	FVector End = MeteorLocation - FVector(0, 0, MeteorLocation.Z + 3000.f);

	FHitResult HitResult;

	if (World->LineTraceSingleByChannel(HitResult, MeteorLocation, End, ECC_WorldDynamic))
	{
		End = HitResult.ImpactPoint;
	}
	else if (World->LineTraceSingleByChannel(HitResult, MeteorLocation, End, ECC_WorldStatic))
	{
		End = HitResult.ImpactPoint;
	}

	auto StartOffset = Settings->StartOffset;
	StartOffset.Z = 0.f;
	MeteorLocation += StartOffset;

	const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(MeteorLocation, End);
	AMeteorShard* MeteorActor = World->SpawnActor<AMeteorShard>(MeteorShardClass, MeteorLocation, Rotation);
	MeteorActor->InitShard(MeteorLocation, End);
	Shards.Add(MeteorActor);
	MeteorActor->OnDestroyed.AddDynamic(this, &UMT_MeteorShards::OnShardDestroyed);
}

FVector UMT_MeteorShards::GetRandomLocation()
{
	const FVector Forward = CharacterActor->GetActorForwardVector();
	const FVector Origin = CharacterActor->GetActorLocation() + Forward * Settings->ForwardDistance;

	const float HeightMin = Settings->HeightMin;
	const float HeightMax = Settings->HeightMax;
	const FVector OffsetRange = Settings->OffsetRange;

	const float x = FMath::RandRange(-OffsetRange.X, OffsetRange.X);
	const float y = FMath::RandRange(-OffsetRange.Y, OffsetRange.Y);
	const float z = FMath::RandRange(HeightMin, HeightMax);

	return FVector(Origin.X + x, y + Origin.Y, z + Origin.Z);
}
