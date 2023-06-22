#include "MT_UIHourGlass.h"

#include "Components/Image.h"
#include "MeteorShower_GP3/MSGameMode.h"
#include "MeteorShower_GP3/Util/Easings.h"

UMT_UIHourGlass::UMT_UIHourGlass()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMT_UIHourGlass::BeginPlay()
{
	Super::BeginPlay();
}

void UMT_UIHourGlass::Register(AMeteorTimer* MeteorTimer)
{
	IMeteorTimed::Register(MeteorTimer);
	auto Character = AMSGameMode::GetCharacter(this);
	if (Character)
	{
		HourglassImage = Character->GameHud->HourglassImage;
	}
}

void UMT_UIHourGlass::OnSetTime(AMeteorTimer* MeteorTimer)
{
	IMeteorTimed::OnSetTime(MeteorTimer);
}

void UMT_UIHourGlass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!HourglassImage)
		return;
	
	if (bRotating)
	{
		RotateTimer += DeltaTime;
		const auto t = RotateTimer / HourglassRotateTime;
		const float Angle = FMath::Lerp(StartAngle, EndAngle, Easings::BackInOut(t));
		HourglassImage->SetRenderTransformAngle(Angle);
		if (RotateTimer >= HourglassRotateTime)
		{
			StopRotate();
		}
	}
	else
	{
		IntervalTimer += DeltaTime;
		if (IntervalTimer >= HourglassRotateInterval)
		{
			StartRotate();
		}
	}
}

void UMT_UIHourGlass::StartRotate()
{
	if (!HourglassImage)
		return;
	
	bRotating = true;
	RotateTimer = 0.f;
	StartAngle = HourglassImage->GetRenderTransformAngle();
	EndAngle = StartAngle + 360;
}

void UMT_UIHourGlass::StopRotate()
{
	bRotating = false;
	IntervalTimer = 0.f;
}
