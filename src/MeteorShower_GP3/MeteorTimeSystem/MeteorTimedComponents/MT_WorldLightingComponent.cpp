#include "MT_WorldLightingComponent.h"

#include "Components/LightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MeteorShower_GP3/MeteorShower_GP3.h"

UMT_WorldLightingComponent::UMT_WorldLightingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMT_WorldLightingComponent::BeginPlay()
{
	Super::BeginPlay();

	const FName LightingTag = FName("Lighting");
	TArray<AActor*> LightingActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), LightingTag, LightingActors);

	for (const auto LightingActor : LightingActors)
	{
		if (ADirectionalLight* DirectionalLightActor = Cast<ADirectionalLight>(LightingActor))
		{
			DirectionalLight = DirectionalLightActor;
			DirectionalLightComponent = DirectionalLight->GetLightComponent();
		}
	}

	if (!DirectionalLight)
	{
		DirectionalLight = Cast<ADirectionalLight>(
			UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
		if (IsValid(DirectionalLight))
		{
			DirectionalLightComponent = DirectionalLight->GetLightComponent();
			LOGW("Add a 'Lighting' ActorTag to DirectionalLight")
		}
	}
}

void UMT_WorldLightingComponent::OnSetTime(AMeteorTimer* MeteorTimer)
{
	IMeteorTimed::OnSetTime(MeteorTimer);

	if (!DirectionalLightComponent)
	{
		return;
	}

	const float ImpactTimer = MeteorTimer->GetTimer();
	const float t = ImpactTimer < TimeLeftWhenToStartLighting
		                ? ImpactTimer / TimeLeftWhenToStartLighting
		                : 1.f;

	const FLinearColor Start = FLinearColor::White;
	const FLinearColor End = FLinearColor::Red;
	const FLinearColor Final = FLinearColor::LerpUsingHSV(Start, End, 1.f - t);

	DirectionalLightComponent->SetLightColor(Final);
}
