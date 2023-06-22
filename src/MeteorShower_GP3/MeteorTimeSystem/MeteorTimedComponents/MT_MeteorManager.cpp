#include "MT_MeteorManager.h"

#include "Kismet/KismetMathLibrary.h"
#include "MeteorShower_GP3/Util/Easings.h"

UMT_MeteorManager::UMT_MeteorManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMT_MeteorManager::BeginPlay()
{
	Super::BeginPlay();

	// const FName LightingTag = FName("Meteor");
	// double TimeStart = FPlatformTime::Seconds();
	// UGameplayStatics::GetAllActorsWithTag(GetWorld(), LightingTag, Meteors);
	// double TimeEnd = FPlatformTime::Seconds();
	// double TimeTotalMs = (TimeEnd - TimeStart) * 1000;
	// LOGIF("Found '%i' meteors in '%.4f'ms.", BigMeteors.Num(), TimeTotalMs)

	UWorld* const World = GetWorld();

	auto Start = Settings->MeteorDestination + FVector(Settings->MeteorXYOffset.X, Settings->MeteorXYOffset.Y, 0.f);
	auto End = Settings->MeteorDestination;
	const FVector MoveDirection = (End - Start).GetSafeNormal();

	const FMeteorDataSettings BigSettings = Settings->BigMeteors;
	for (int i = 0; i < Settings->BigMeteorCount; ++i)
	{
		AActor* Meteor = World->SpawnActor(Settings->MeteorClass);
		Meteor->SetActorScale3D(FVector(BigSettings.Scale));
		BigMeteors.Add(Meteor);
		FVector MeteorOffset = FVector(
			FMath::RandRange(-BigSettings.MeteorRandomDeviation.X, BigSettings.MeteorRandomDeviation.X),
			FMath::RandRange(-BigSettings.MeteorRandomDeviation.Y, BigSettings.MeteorRandomDeviation.Y),
			FMath::RandRange(-BigSettings.MeteorRandomDeviation.Z, BigSettings.MeteorRandomDeviation.Z));
		BigMeteorConfigs.Add(FMeteorConfig(MeteorOffset));
	}

	const FMeteorDataSettings SmallSettings = Settings->SmallMeteors;
	for (int i = 0; i < Settings->SmallMeteorCount; ++i)
	{
		AActor* Meteor = World->SpawnActor(Settings->MeteorClass);
		Meteor->SetActorScale3D(FVector(SmallSettings.Scale * FMath::FRandRange(0.5f, 3.f)));
		SmallMeteors.Add(Meteor);
		FVector MeteorOffset = FVector(
			FMath::RandRange(-SmallSettings.MeteorRandomDeviation.X, SmallSettings.MeteorRandomDeviation.X),
			FMath::RandRange(-SmallSettings.MeteorRandomDeviation.Y, SmallSettings.MeteorRandomDeviation.Y),
			FMath::RandRange(-SmallSettings.MeteorRandomDeviation.Z, SmallSettings.MeteorRandomDeviation.Z));
		FVector DestinationOffset =
			MoveDirection * FMath::RandRange(1.f, 150000.f);
		MeteorOffset -= MoveDirection * FMath::RandRange(1.f, 150000.f);
		auto config = FMeteorConfig(MeteorOffset);
		config.DestinationOffset = DestinationOffset;
		SmallMeteorConfigs.Add(config);
	}

	SetMeteorShowerProgress(1.f);
}

void UMT_MeteorManager::OnSetTime(AMeteorTimer* MeteorTimer)
{
	IMeteorTimed::OnSetTime(MeteorTimer);
	const float TimeToImpact = MeteorTimer->GetTimer();
	const float MeteorProgress = TimeToImpact < Settings->TimeLeftWhenToMoveMeteors
		                             ? TimeToImpact / Settings->TimeLeftWhenToMoveMeteors
		                             : 1.f;
	SetMeteorShowerProgress(MeteorProgress);
}

void UMT_MeteorManager::SetMeteorShowerProgress(float MeteorProgress)
{
	float t = Easings::ExpoOut(MeteorProgress);
	HandleMeteors(BigMeteors, BigMeteorConfigs, t, Settings->BigMeteors);
	HandleMeteors(SmallMeteors, SmallMeteorConfigs, t, Settings->SmallMeteors);
}

void UMT_MeteorManager::HandleMeteors(TArray<AActor*> MeteorsToHandle, TArray<FMeteorConfig> MeteorConfigs,
                                      float MeteorProgress, const FMeteorDataSettings& MeteorSettings)
{
	FVector Start = FVector(
		Settings->MeteorDestination.X + Settings->MeteorXYOffset.X,
		Settings->MeteorDestination.Y + Settings->MeteorXYOffset.Y,
		Settings->MeteorStartZ);

	for (int i = 0; i < MeteorsToHandle.Num(); ++i)
	{
		AActor* Meteor = MeteorsToHandle[i];
		const FMeteorConfig Config = MeteorConfigs[i];
		FVector End = Settings->MeteorDestination + Config.DestinationOffset;
		FVector MeteorLocation = FMath::Lerp(End, Start, MeteorProgress);
		MeteorLocation += Config.Offset;
		Meteor->SetActorLocation(MeteorLocation);
		const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
		Meteor->SetActorRotation(Rotation);
	}
}
