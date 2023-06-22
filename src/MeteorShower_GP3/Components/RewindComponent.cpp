#include "RewindComponent.h"

#include "MeteorShower_GP3/MeteorShower_GP3.h"

URewindComponent::URewindComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URewindComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();

	if (const APawn* OwnerPawn = Cast<APawn>(Owner))
		Controller = OwnerPawn->GetController();

	TimerManager = &GetWorld()->GetTimerManager();
	SetTimelineLength(SecondsToRecord);
	// LOG("Rewind ready");
	StartRecord();
}


void URewindComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bRewinding)
	{
		SnapTimer += DeltaTime;
		if (SnapTimer >= TimePerSnap)
		{
			if (CurrentIndex < 0)
			{
				RewindFinished();
				return;
			}
			SnapTimer = 0.f;
			CurrentSnap = GenerateSnapshot();
			NextSnap = Timeline[CurrentIndex--];
		}
		const float t = SnapTimer / TimePerSnap;
		const UE::Math::TVector<double> Location = FMath::Lerp(CurrentSnap.Location, NextSnap.Location, t);
		const UE::Math::TRotator<double> Rotation = FMath::Lerp(CurrentSnap.Rotation, NextSnap.Rotation, t);

		Owner->SetActorLocation(Location);
		if (Controller)
			Controller->SetControlRotation(Rotation);
		else
			Owner->SetActorRotation(Rotation);
	}
}

void URewindComponent::StartRecord()
{
	if (bRewinding)
	{
		LOG("Cant start record while rewinding");
		return;
	}

	// LOG("Recording");
	TimerManager->SetTimer(SnapshotTimerHandle, this, &URewindComponent::TakeSnapshot, RecordDelta, true);
}

void URewindComponent::PauseRecord()
{
	TimerManager->ClearTimer(SnapshotTimerHandle);
}

void URewindComponent::StopRecord()
{
	if (bRewinding)
	{
		LOG("Cant stop record while rewinding");
		return;
	}

	// LOG("Stopped recording");
	TimerManager->ClearTimer(SnapshotTimerHandle);
	ResetTimeline();
}

void URewindComponent::TakeSnapshot()
{
	if (bRewinding)
	{
		LOG("Cant take snapshot while rewinding");
		return;
	}

	Timeline.Add(GenerateSnapshot());
	if (AutoTriggerWhenDoneRecording && Timeline.Num() >= RecordSize)
	{
		StartRewind();
	}
}

void URewindComponent::ResetTimeline()
{
	if (bRewinding)
	{
		LOG("Cant reset timeline while rewinding");
		return;
	}

	Timeline.Empty();
}

void URewindComponent::SetTimelineLength(float InSecondsToRecord)
{
	SecondsToRecord = InSecondsToRecord;
	RecordSize = SecondsToRecord / RecordDelta + .5f;
}

void URewindComponent::StartRewind()
{
	if (bRewinding)
	{
		LOG("Cant rewind while already rewinding");
		return;
	}

	// LOG("Rewinding");
	PauseRecord();

	bRewinding = true;

	SnapTimer = 0.f;
	TimePerSnap = RewindTime / (Timeline.Num() + 1);

	// LOGIF("time per snap: %f", TimePerSnap);
	CurrentSnap = GenerateSnapshot();

	CurrentIndex = Timeline.Num() - 1;
	NextSnap = Timeline[CurrentIndex--];

	SetComponentTickEnabled(true);

	OnRewindStartedEvent.Broadcast();
}

void URewindComponent::RewindFinished()
{
	if (!bRewinding)
	{
		LOG("Cant finish rewinding while not rewinding (RewindFinished())");
	}
	bRewinding = false;
	SetComponentTickEnabled(false);
	StopRecord();
	OnRewindFinishedEvent.Broadcast();
	StartRecord();
}

FRewindSnapshot URewindComponent::GenerateSnapshot() const
{
	const FVector SnapLocation = Owner->GetActorLocation();
	const FRotator SnapRotation = Owner->GetActorRotation();
	return FRewindSnapshot(SnapLocation, SnapRotation);
}