// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/TimelineComponent.h"
#include "Components/Image.h"

#include "ScreenFader.generated.h"
/*
 *
 */
UCLASS()
class METEORSHOWER_GP3_API UScreenFader : public UImage
{
	GENERATED_BODY()

protected:
	virtual void PostInitProperties() override;
	
public:

	UFUNCTION(BlueprintCallable)
	void StartFade(float FadeTime, float OpacityStartValue, float OpacityEndValue);
	
	UFUNCTION(BlueprintCallable)
	void SetFadeOpacity(float Opacity);

private:
	float FadeTime{1.f};
	float EndValue;
	float StartValue;
	
	float ElapsedTime;
	float Delta{0.0166f};
	
	UPROPERTY()
	FTimeline Timeline;

	UFUNCTION()
	void OnTimelineFinished();

	UFUNCTION()
	void TimelineUpdate(float Value);

	UFUNCTION()
	void TimelineTick();

	UPROPERTY()
	FTimerHandle TickTimerHandle;
};
