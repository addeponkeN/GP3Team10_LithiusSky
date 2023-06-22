#include "TimeGunLaser.h"

ATimeGunLaser::ATimeGunLaser()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	// NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("NiagaraBeam"));
	// NiagaraComponent->SetupAttachment(RootComponent);
}

void ATimeGunLaser::BeginPlay()
{
	Super::BeginPlay();
	NiagaraComponent = Cast<UNiagaraComponent>(GetComponentByClass(UNiagaraComponent::StaticClass()));
	if (NiagaraComponent)
	{
		NiagaraComponent->SetAsset(NiagaraSystem);
		NiagaraComponent->Activate(true);
	}
	Arrow->SetHiddenInGame(true);
}

void ATimeGunLaser::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);
	const FVector BeamEnd = GetBeamEnd();
	NiagaraComponent->SetNiagaraVariableVec3(BeamVariableEndName, BeamEnd);
}

void ATimeGunLaser::SetDestination(FVector const& Destination)
{
	Super::SetDestination(Destination);
	
	DestinationLocation = Destination;
	const FVector BeamEnd = GetBeamEnd();
	NiagaraComponent->SetNiagaraVariableVec3(BeamVariableEndName, BeamEnd);
}

FVector ATimeGunLaser::GetBeamEnd()
{
	const FTransform ComponentTransform = NiagaraComponent->GetComponentTransform();
	const FVector FinalDestination = ComponentTransform.InverseTransformPosition(DestinationLocation);
	return FinalDestination;
}
