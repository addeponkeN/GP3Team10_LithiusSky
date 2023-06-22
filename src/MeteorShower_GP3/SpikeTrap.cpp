#include "SpikeTrap.h"

#include "MeteorShower_GP3.h"
#include "MeteorShower_GP3Character.h"

ASpikeTrap::ASpikeTrap()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	RootComponent = Mesh;

	HitBox = CreateDefaultSubobject<UBoxComponent>(FName("HitBox"));
	HitBox->SetupAttachment(Mesh);
}

void ASpikeTrap::BeginPlay()
{
	Super::BeginPlay();
	TimerManager = &GetWorld()->GetTimerManager();
	if (Settings)
	{
		HitBox->OnComponentBeginOverlap.AddDynamic(this, &ASpikeTrap::OnOverlapBegin);
	}
	else
	{
		LOGWF("SpikeTrap '%s' have no settings", *GetName());
	}
	SetActorTickEnabled(false);
}

void ASpikeTrap::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	if (State == ESpikeTrapState::SpikeTriggered)
	{
		TriggerTimer += DeltaTime;
		if (TriggerTimer > Settings->TriggerTime)
		{
			OnAttacking();
			SetActorTickEnabled(false);
		}
		// LOGF("Triggering: %.3f s", TriggerTimer);
	}
}

void ASpikeTrap::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// LOGF("Overlap: %s", *OtherActor->GetName());
	if (AMeteorShower_GP3Character* OverlappedCharacter = Cast<AMeteorShower_GP3Character>(OtherActor))
	{
		Character = OverlappedCharacter;
		UpdateState();
		HitBox->OnComponentEndOverlap.AddDynamic(this, &ASpikeTrap::ASpikeTrap::OnOverlapEnd);
		// LOG("set character");
	}
}

void ASpikeTrap::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (const AMeteorShower_GP3Character* OverlappedCharacter = Cast<AMeteorShower_GP3Character>(OtherActor))
	{
		if (OverlappedCharacter == Character)
		{
			HitBox->OnComponentEndOverlap.RemoveDynamic(this, &ASpikeTrap::ASpikeTrap::OnOverlapEnd);
			Character = nullptr;
			// LOG("removed character");
		}
	}
}

void ASpikeTrap::UpdateState()
{
	if (State == ESpikeTrapState::SpikeIdle)
	{
		OnTrigger();
	}
}

void ASpikeTrap::OnTrigger()
{
	// LOG("triggered");
	//	spikes triggered, about to emerge
	State = ESpikeTrapState::SpikeTriggered;
	SetActorTickEnabled(true);
	TriggerTimer = 0.f;
}

void ASpikeTrap::OnAttacking()
{
	// LOG("attacking");
	//	spikes go out real fast, check collision & kill player
	State = ESpikeTrapState::SpikeAttacking;

	if (Character)
	{
		// LOG("SPIKE HIT CHARACTER");
		Character->HealthComponent->Kill();
		Character = nullptr;
	}

	TimerManager->SetTimer(UpdateTimerHandle, this, &ASpikeTrap::OnCooling, Settings->WaitTime);
}

void ASpikeTrap::OnCooling()
{
	// LOG("cooling");
	//	spikes go down...
	State = ESpikeTrapState::SpikeCooling;

	TimerManager->SetTimer(UpdateTimerHandle, this, &ASpikeTrap::SetIdle, Settings->SubmergeTime);
}

void ASpikeTrap::SetIdle()
{
	//	spikes idle
	// LOG("idle");
	State = ESpikeTrapState::SpikeIdle;
}
