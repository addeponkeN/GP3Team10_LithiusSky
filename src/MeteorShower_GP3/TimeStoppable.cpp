#include "TimeStoppable.h"

#include "MeteorShower_GP3.h"
#include "Util/Easings.h"
#include "NiagaraFunctionLibrary.h"
#include "SoundHelper.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"

FTimeStopStatsSnapshot FTimeStopStatsSnapshot::Default{};

UTimeStoppable::UTimeStoppable()
{
	PrimaryComponentTick.bCanEverTick = true;
	// UPrimitiveComponent::SetCollisionProfileName(TEXT("TimeStoppable"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> TimeStoppable(
		TEXT("/Game/TestFolders/Andreas/FX/NS_TimeStopped.NS_TimeStopped"));
	if (TimeStoppable.Succeeded())
	{
		TimeStoppableSystem = TimeStoppable.Object;
	}

	const wchar_t* TimeStoppableDataPath = TEXT("/Game/Settings/DA_TimeStoppableSettings.DA_TimeStoppableSettings");
	static ConstructorHelpers::FObjectFinder<UTimeStoppableDataAsset>
		TimeStoppableDataAsset(TimeStoppableDataPath);
	if (TimeStoppableDataAsset.Succeeded())
	{
		Settings = TimeStoppableDataAsset.Object;
	}
	else
	{
		LOGEF("Could not find TimeGun Data Asset '%s'", TimeStoppableDataPath);
	}

	// UStaticMeshComponent* MeshParent = Cast<UStaticMeshComponent>(GetAttachParent());
	// if (MeshParent)
	// {
	// auto bounds = MeshParent->GetStaticMesh()->GetBounds();
	// SetBoxExtent(bounds.BoxExtent);
	// }
}

void UTimeStoppable::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
	OwnerActor = GetOwner();
	Root = OwnerActor->GetRootComponent();
	// Root = GetAttachmentRoot();
	MeshRoot = Cast<UStaticMeshComponent>(Root);

	if (bOnlyJuiceMeshParent)
	{
		if (MeshRoot)
		{
			AddSlowMo(MeshRoot);
		}
	}
	else
	{
		ChildMeshes = TArray<UStaticMeshComponent*>();
		OwnerActor->GetComponents<UStaticMeshComponent>(ChildMeshes);
		for (UStaticMeshComponent* Mesh : ChildMeshes)
		{
			if (!Mesh)
			{
				continue;
			}
			AddSlowMo(Mesh);
		}
	}
	TimerManager = &GetWorld()->GetTimerManager();
	ProcessTimer = 1.f;
	SlowAmountStart = 1.f;
	bDoneProcess = true;
	GravityZ = GetWorld()->GetGravityZ();

	TArray<UStaticMeshComponent*> Components;
	GetOwner()->GetComponents<UStaticMeshComponent>(Components);
	//UStaticMeshComponent* RootMesh = Components[0];

	for (UStaticMeshComponent* sm : Components)
	{
		const auto InstantiatedMaterial = UMaterialInstanceDynamic::Create(sm->GetMaterial(0), this);
		sm->SetMaterial(0, InstantiatedMaterial);
		InstantiatedMaterial->SetScalarParameterValue("CutOff_Alpha", 0.0f);
		InstantiatedMaterial->SetScalarParameterValue("CutOff_Transition", 0.0f);
		TransitionMaterials.Add(InstantiatedMaterial);
	}

	/*TransitionMaterial = UMaterialInstanceDynamic::Create(RootMesh->GetMaterial(0), this);
	RootMesh->SetMaterial(0, TransitionMaterial);
	TransitionMaterial->SetScalarParameterValue("CutOff_Alpha", 0.0f);
	TransitionMaterial->SetScalarParameterValue("CutOff_Transition", 0.0f);*/
}

void UTimeStoppable::AddSlowMo(UStaticMeshComponent* Mesh)
{
	const FName AttachName = FName(FString::Printf(TEXT("SlowMoEffect_%s"), *Mesh->GetName()));
	UNiagaraComponent* const NiagaraComponent =
		UNiagaraFunctionLibrary::SpawnSystemAttached(TimeStoppableSystem, Mesh, AttachName,
		                                             FVector::ZeroVector, FRotator::ZeroRotator,
		                                             EAttachLocation::SnapToTargetIncludingScale, /* bAutoDestroy */
		                                             false, /* bAutoActivate */ false);

	NiagaraComponent->SetVariableStaticMesh(TEXT("ParticleMesh"), Mesh->GetStaticMesh());
	NiagaraComponent->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	SlowMos.Add(NiagaraComponent);
}

void UTimeStoppable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bMeshRefPhysics)
	{
		Velocity.Z += GravityZ * SlowAmount * DeltaTime;
		MeshRoot->SetPhysicsLinearVelocity(Velocity * SlowAmount);
		MeshRoot->SetPhysicsAngularVelocityInRadians(Angular * SlowAmount);
	}
}

/**
 * @brief Slows down the targeted mesh by lowering CustomTimeDilation & overriding physics velocity
 * @param pSlowDuration seconds to freeze
 * @param pSlowAmount amount of slow effect (lower is slower)
 */
void UTimeStoppable::Freeze(float pSlowDuration, float pSlowAmount)
{
	if (bFrozen)
	{
		return;
	}

	// LOG("freeze");

	// LOG("SLOWED");
	pSlowAmount *= SlowAmountScale;

	DesiredSlowAmount = pSlowAmount;
	SlowAmountStart = 1.f;
	SlowAmountEnd = pSlowAmount;

	GravityZ = GetWorld()->GetGravityZ();

	bFrozen = true;
	StartFreeze();

	for (auto SlowMo : SlowMos)
	{
		SlowMo->Activate(true);
	}

	Snapshot = FTimeStopStatsSnapshot::Default;
	Snapshot.CustomTimeDilation = OwnerActor->CustomTimeDilation;

	// if the timestoppable has a mesh and physics enabled > enable slowdown physics
	bMeshRefPhysics = MeshRoot && MeshRoot->IsSimulatingPhysics();

	if (bMeshRefPhysics)
	{
		Snapshot.bGravityEnabled = MeshRoot->IsGravityEnabled();
		Velocity = MeshRoot->GetPhysicsLinearVelocity();
		Angular = MeshRoot->GetPhysicsAngularVelocityInRadians();
		MeshRoot->SetEnableGravity(false);
	}

	SetComponentTickEnabled(true);

	TimerManager->SetTimer(TimerHandle, this, &UTimeStoppable::StartUnFreeze_Impl, pSlowDuration);
	OnStoppedEvent.Broadcast();

	if (!TimerManager->TimerExists(TickTimeIndependentTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(TickTimeIndependentTimerHandle, this,
		                                       &UTimeStoppable::Tick_TimeIndependent, UpdateSlowDelta, true, -1);
	}
	AnimationState = AnimationStates_Time::In;
}


/**
 * @brief turns on SimulatePhysics on the mesh
 */
void UTimeStoppable::UnFreeze()
{
	// LOG("unfreeze");
	TimerManager->ClearTimer(SlowAmountUpdateTimerHandle);
	bFrozen = false;
	OwnerActor->CustomTimeDilation = 1.f;
	if (bMeshRefPhysics)
	{
		MeshRoot->SetPhysicsLinearVelocity(Velocity * 0.9f);
		MeshRoot->SetPhysicsAngularVelocityInRadians(Angular * 0.9f);
		MeshRoot->SetEnableGravity(Snapshot.bGravityEnabled);
	}
	for (auto SlowMo : SlowMos)
	{
		SlowMo->Deactivate();
	}
	SetComponentTickEnabled(false);
}

void UTimeStoppable::StartFreeze()
{
	if (bDoneProcess)
		TimerManager->SetTimer(SlowAmountUpdateTimerHandle, this, &UTimeStoppable::UpdateSlowAmount, UpdateSlowDelta,
		                       true);

	if (Settings && Settings->OnStoppedSound)
	{
		// float SfxVolume = USoundHelper::GetSFXVolume(GetWorld());
		// USoundAttenuation* SoundAtt = NewObject<USoundAttenuation>();
		// SoundAtt->Attenuation.RadiusMin_DEPRECATED = 1000.f;
		// SoundAtt->Attenuation.RadiusMax_DEPRECATED = 8000.f;
		// SoundAtt->Attenuation.ConeSphereRadius = 1000.f;
		// SoundAtt->Attenuation.ConeSphereFalloffDistance = 8000.f;
		// UGameplayStatics::PlaySoundAtLocation(this, Settings->OnStoppedSound,
		// Root->GetComponentLocation(), FRotator::ZeroRotator, SfxVolume, 1.f, 0.f, SoundAtt);
		USoundHelper::PlaySoundSFX(this, Settings->OnStoppedSound);
	}

	ProcessTimerMultiplier = 1.f;
	ProcessTimer = ProcessTime - ProcessTimer;
	bDoneProcess = false;
	bEntering = true;
	AnimationState = AnimationStates_Time::In;
	TransitionTimer_Internal = 0;
	// LOG("start freeze");
}

void UTimeStoppable::StartUnFreeze(bool ImmediateUnFreeze)
{
	if (!bFrozen)
	{
		return;
	}

	bFrozen = false;
	StartUnFreeze_Impl();
	ProcessTimerMultiplier = ImmediateUnFreeze ? 3.f : 0.25f;
	// LOG("start unfreeze");
}

void UTimeStoppable::StartUnFreeze_Impl()
{
	TimerManager->ClearTimer(TimerHandle);
	if (bDoneProcess)
		TimerManager->SetTimer(SlowAmountUpdateTimerHandle, this, &UTimeStoppable::UpdateSlowAmount, UpdateSlowDelta,
		                       true);
	bDoneProcess = false;
	bEntering = false;
	ProcessTimerMultiplier = 0.25f;
	ProcessTimer = ProcessTime - ProcessTimer;
	SlowAmountStart = DesiredSlowAmount;
	SlowAmountEnd = 1.f;
	TransitionTimer_Internal = 0;
	AnimationState = AnimationStates_Time::Out;

	if (Settings && Settings->OnResumedSound)
	{
		float SfxVolume = USoundHelper::GetSFXVolume(GetWorld());
		USoundAttenuation* SoundAtt = NewObject<USoundAttenuation>();
		SoundAtt->Attenuation.ConeSphereRadius = 1000.f;
		SoundAtt->Attenuation.ConeSphereFalloffDistance = 8000.f;
		UGameplayStatics::PlaySoundAtLocation(this, Settings->OnResumedSound,
		                                      Root->GetComponentLocation(), FRotator::ZeroRotator, SfxVolume, 1.f, 0.f,
		                                      SoundAtt);
	}
	// LOG("start unfreeze impl (play sound)");

	OnResumedEvent.Broadcast();
}

void UTimeStoppable::OnLookBegin()
{
	if (MeshRoot)
	{
		for (auto Mesh : ChildMeshes)
		{
			Mesh->SetRenderCustomDepth(true);
		}
	}
	OnLookBeginEvent.Broadcast();
}

void UTimeStoppable::OnLookEnd()
{
	if (MeshRoot)
	{
		for (auto Mesh : ChildMeshes)
		{
			Mesh->SetRenderCustomDepth(false);
		}
	}
	OnLookEndEvent.Broadcast();
}

void UTimeStoppable::UpdateSlowAmount()
{
	ProcessTimer = FMath::Clamp(ProcessTimer + UpdateSlowDelta * ProcessTimerMultiplier, 0.f, ProcessTime);
	float p = ProcessTimer / ProcessTime;
	float t = bEntering ? Easings::CubicOut(p) : Easings::CubicIn(p);
	SlowAmount = FMath::Lerp(SlowAmountStart, SlowAmountEnd, t);
	OwnerActor->CustomTimeDilation = 1.f * SlowAmount;
	if (ProcessTimer >= ProcessTime)
	{
		bDoneProcess = true;
		TimerManager->ClearTimer(SlowAmountUpdateTimerHandle);
		if (!bEntering)
		{
			UnFreeze();
			// LOG("unfreeze in update");
		}
		bEntering = false;
	}
}

void UTimeStoppable::Tick_TimeIndependent()
{
	//Animate between timestop states
	float TransitionValue;
	for (UMaterialInstanceDynamic* Material : TransitionMaterials)
	{
		switch (AnimationState)
		{
		case AnimationStates_Time::In:

			TransitionTimer_Internal += TimerManager->GetTimerElapsed(TickTimeIndependentTimerHandle);
			TransitionValue = (EasingIn != nullptr)
				                  ? EasingIn->GetFloatValue(TransitionTimer_Internal / InTransitionTime)
				                  : TransitionTimer_Internal / InTransitionTime;
			Material->SetScalarParameterValue("CutOff_Transition", TransitionValue);
			if (TransitionTimer_Internal >= InTransitionTime)
			{
				Material->SetScalarParameterValue("CutOff_Transition", 1.f);
				TransitionTimer_Internal = 0;
				AnimationState = AnimationStates_Time::Stay;
			}
			break;
		case AnimationStates_Time::Stay:
			break;
		case AnimationStates_Time::Out:

			TransitionTimer_Internal += TimerManager->GetTimerElapsed(TickTimeIndependentTimerHandle);
			TransitionValue = (EasingOut != nullptr)
				                  ? EasingOut->GetFloatValue(TransitionTimer_Internal / OutTransitionTime)
				                  : TransitionTimer_Internal / OutTransitionTime;
			Material->SetScalarParameterValue("CutOff_Transition", 1.f - TransitionValue);
			if (TransitionTimer_Internal >= InTransitionTime)
			{
				Material->SetScalarParameterValue("CutOff_Transition", 0.f);
				TransitionTimer_Internal = 0;
				AnimationState = AnimationStates_Time::Stay;
			}
			break;
		}
	}
}
