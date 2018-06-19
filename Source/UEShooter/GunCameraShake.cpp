// Copyright Sean Duffy 2018

#include "GunCameraShake.h"




UGunCameraShake::UGunCameraShake()
{
	OscillationDuration = 0.15f;
	OscillationBlendInTime = 0.025f;
	OscillationBlendOutTime = 0.025f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(0.15f, 0.35f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(0.5f, 1.5f);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(0.15f, 0.35f);
	RotOscillation.Yaw.Frequency = FMath::RandRange(0.5f, 1.5f);
}
