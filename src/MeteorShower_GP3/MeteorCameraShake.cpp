// Fill out your copyright notice in the Description page of Project Settings.


#include "MeteorCameraShake.h"

UMeteorCameraShake::UMeteorCameraShake()
{
	OscillationDuration = 0.5f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	constexpr float RotAmpMin = 0.5f;
	constexpr float RotAmpMax = 1.0f;

	constexpr float RotFreqMin = 5.0f;
	constexpr float RotFreqMax = 10.0f;
	
	RotOscillation.Pitch.Amplitude = FMath::RandRange(RotAmpMin, RotAmpMax);
	RotOscillation.Pitch.Frequency = FMath::RandRange(RotFreqMin, RotFreqMax);
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;

	RotOscillation.Yaw.Amplitude = FMath::RandRange(RotAmpMin,RotAmpMax);
	RotOscillation.Yaw.Frequency = FMath::RandRange(RotFreqMin, RotFreqMax);
	RotOscillation.Yaw.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;

	LocOscillation.Z.Amplitude = FMath::RandRange(3.0f, 6.0f);
	LocOscillation.Z.Frequency = FMath::RandRange(20.0f, 40.0f);
	LocOscillation.Z.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
}
