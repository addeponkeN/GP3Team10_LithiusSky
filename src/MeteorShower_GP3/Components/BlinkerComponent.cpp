#include "BlinkerComponent.h"

#include "MeteorShower_GP3/MeteorShower_GP3.h"

UBlinkerComponent::UBlinkerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBlinkerComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerActor = GetOwner();
	TimeStoppable = Cast<UTimeStoppable>(OwnerActor->GetComponentByClass(UTimeStoppable::StaticClass()));
	OwnerActor->GetComponents(Meshes);
	StartBlinking();
}

void UBlinkerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	timer += DeltaTime;

	float TransitionValue;
	switch (AnimationState)
	{
	case AnimationStates::In:

		TransitionValue = (EasingInVisible != nullptr)
			                  ? 1 - EasingInVisible->GetFloatValue(timer / VisibleTransitionTime)
			                  : 1 - timer / VisibleTransitionTime;

		SetTransitionMaterialValue(TransitionValue);

		if (timer >= VisibleTransitionTime)
		{
			timer = 0;
			AnimationState = AnimationStates::StayVisible;
			SetTransitionMaterialValue(0.f);
			SetStateVisible();
		}

		break;

	case AnimationStates::StayVisible:

		if (timer >= VisibleStayTime)
		{
			timer = 0;
			AnimationState = AnimationStates::Out;
			if (!bAllowColliderOnTransitionHidden)
			{
				GetOwner()->SetActorEnableCollision(false);
			}
		}

		break;

	case AnimationStates::Out:

		TransitionValue = (EasingOutHidden != nullptr)
			                  ? EasingOutHidden->GetFloatValue(timer / HiddenTransitionTime)
			                  : timer / HiddenTransitionTime;

		SetTransitionMaterialValue(TransitionValue);

		if (timer >= HiddenTransitionTime)
		{
			if (bAllowColliderOnTransitionHidden)
			{
				GetOwner()->SetActorEnableCollision(false);
			}

			timer = 0;
			AnimationState = AnimationStates::StayHidden;
			SetTransitionMaterialValue(1.f);
			SetStateHidden();
		}

		break;

	case AnimationStates::StayHidden:

		if (timer >= HiddenStayTime)
		{
			timer = 0;
			AnimationState = AnimationStates::In;
			GetOwner()->SetActorEnableCollision(true);
		}

		break;
	}
}

void UBlinkerComponent::StartBlinking()
{
}

void UBlinkerComponent::SetIsVisible(bool IsVisible)
{
	bVisible = IsVisible;
}

void UBlinkerComponent::SetStartVisible()
{
}

void UBlinkerComponent::SetStateVisible()
{
	bVisible = true;
}

void UBlinkerComponent::SetStartHidden()
{
	
}
void UBlinkerComponent::SetStateHidden()
{
	bVisible = false;
	if (TimeStoppable)
	{
		TimeStoppable->StartUnFreeze(true);
	}
}

void UBlinkerComponent::SetTransitionMaterialValue(float Value)
{
	if (!TransitionMaterial)
	{
		TransitionMaterial = TimeStoppable->TransitionMaterials[0];
	}
	TransitionMaterial->SetScalarParameterValue("CutOff_Alpha", Value);
}
