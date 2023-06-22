#include "MT_CameraShake.h"

#include "MeteorShower_GP3/MeteorCameraShake.h"
#include "MeteorShower_GP3/MeteorShower_GP3.h"
#include "MeteorShower_GP3/Util/Easings.h"

UMT_CameraShake::UMT_CameraShake()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMT_CameraShake::BeginPlay()
{
	Super::BeginPlay();
	Shake = NewObject<UMeteorCameraShake>(GetTransientPackage(), FName("MeteorCameraShaker"));
	CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
}

void UMT_CameraShake::OnSetTime(AMeteorTimer* MeteorTimer)
{
	IMeteorTimed::OnSetTime(MeteorTimer);

	if (!CameraManager)
	{
		LOGW("CameraManager null");
		return;
	}

	if (!Shake)
	{
		LOGW("shake null");
		return;
	}

	const float ImpactTimer = MeteorTimer->GetTimer();
	auto Settings = MeteorTimer->Settings;

	if (bShaking)
	{
		CameraManager->StopAllCameraShakes(false);
		bShaking = false;
		return;
	}

	if (ImpactTimer < Settings->SecondsLeftWhenToShake)
	{
		const float Percent = MeteorTimer->GetTimer() / Settings->SecondsLeftWhenToShake;
		const float FinalShakeAmount = FMath::Lerp(Settings->ShakeAmount, 0.f, Easings::CubicOut(Percent));

		//	TODO : improve this shiet, cache the ret value from StartCameraShake

		// CameraManager->PlayWorldCameraShake(GetWorld(), Shake->GetClass(), Center, 0.f, 35.f, false);
		CameraManager->StartCameraShake(Shake->GetClass(), FinalShakeAmount);
		// Shake->StartLegacyCameraShake(CameraManager, Shake->GetClass(), ShakeAmount);

		// Shake->StartShake(CameraManager, ShakeAmount, ECameraShakePlaySpace::CameraLocal);

		bShaking = true;
		// LOG("shaking");
	}
}
