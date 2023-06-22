#include "ScreenFader.h"

#include "Components/CanvasPanelSlot.h"

void UScreenFader::PostInitProperties()
{
	Super::PostInitProperties();

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
	{
		CanvasSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
	}

	const FLinearColor Clr = ColorAndOpacity;
	SetColorAndOpacity(FLinearColor(Clr.R, Clr.G, Clr.B, 0.f));

	FOnTimelineFloat TimelineCallback;
	TimelineCallback.BindUFunction(this, FName("TimelineUpdate"));

	FOnTimelineEvent TimelineFinishedEvent;
	TimelineFinishedEvent.BindUFunction(this, TEXT("OnTimelineFinished"));

	Timeline.SetLooping(false);

	// Timeline.AddInterpFloat(Curve, TimelineCallback);
	// Timeline.SetTimelineFinishedFunc(TimelineFinishedEvent);
}

/**
 * @brief 0=visible,  1=dark
 * @param OpacityFadeTime Duration of the fade
 * @param OpacityStartValue Opacity start value
 * @param OpacityEndValue Opacity end value
 */
void UScreenFader::StartFade(float OpacityFadeTime, float OpacityStartValue, float OpacityEndValue)
{
	ElapsedTime = 0.f;

	this->FadeTime = OpacityFadeTime;
	this->StartValue = OpacityStartValue;
	this->EndValue = OpacityEndValue;

	Timeline.SetTimelineLength(FadeTime);
	Timeline.PlayFromStart();

	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &UScreenFader::TimelineTick, Delta, true);
}

void UScreenFader::SetFadeOpacity(const float Opacity)
{
	SetOpacity(Opacity);
}

void UScreenFader::OnTimelineFinished()
{
	GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);
	SetOpacity(EndValue);
}

void UScreenFader::TimelineUpdate(const float Value)
{
	SetOpacity(Value);
}

void UScreenFader::TimelineTick()
{
	if (Timeline.IsPlaying())
	{
		ElapsedTime += Delta;
		const float Alpha = FMath::Clamp(ElapsedTime / FadeTime, 0.f, 1.f);
		const float Value = FMath::Lerp(StartValue, EndValue, Alpha);
		TimelineUpdate(Value);
		if (Alpha >= 1.f)
		{
			OnTimelineFinished();
			Timeline.Stop();
		}
	}
}
