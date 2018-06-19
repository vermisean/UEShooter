// Copyright Sean Duffy 2018

#include "DamageCameraShake.h"




UDamageCameraShake::UDamageCameraShake()
{
	OscillationDuration = 0.35f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(0.55f, 1.35f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(0.55f, 1.5f);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(0.55f, 1.35f);
	RotOscillation.Yaw.Frequency = FMath::RandRange(0.55f, 1.5f);
}
