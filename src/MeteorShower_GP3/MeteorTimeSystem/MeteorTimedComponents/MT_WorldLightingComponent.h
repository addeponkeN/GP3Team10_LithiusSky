// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DirectionalLight.h"
#include "MeteorShower_GP3/MeteorTimeSystem/MeteorTimed.h"
#include "MT_WorldLightingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class METEORSHOWER_GP3_API UMT_WorldLightingComponent : public UActorComponent, public IMeteorTimed
{
	GENERATED_BODY()

public:	
	UMT_WorldLightingComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnSetTime(AMeteorTimer* MeteorTimer) override;

private:

	UPROPERTY(EditAnywhere)
	float TimeLeftWhenToStartLighting{60.f};
	
	UPROPERTY(VisibleAnywhere)
	ADirectionalLight* DirectionalLight;
	
	UPROPERTY(VisibleAnywhere)
	ULightComponent* DirectionalLightComponent;
};
