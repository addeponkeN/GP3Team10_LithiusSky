// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeteorShower_GP3/MeteorTimeSystem/MeteorTimed.h"
#include "MT_WhiteScreenFade.generated.h"


class UScreenFader;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class METEORSHOWER_GP3_API UMT_WhiteScreenFade : public UActorComponent, public IMeteorTimed
{
	GENERATED_BODY()

public:
	UMT_WhiteScreenFade();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	virtual void OnSetTime(AMeteorTimer* MeteorTimer) override;

private:
	UPROPERTY()
	UScreenFader* Fader;
};
