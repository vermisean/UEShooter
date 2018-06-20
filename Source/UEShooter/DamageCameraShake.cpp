// Copyright Sean Duffy 2018

#include "DamageCameraShake.h"




UDamageCameraShake::UDamageCameraShake()
{
	OscillationDuration = 0.65f;
	OscillationBlendInTime = 0.5f;
	OscillationBlendOutTime = 0.5f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(5.55f, 10.35f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(5.55f, 10.5f);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(5.55f, 10.35f);
	RotOscillation.Yaw.Frequency = FMath::RandRange(5.55f, 10.5f);
}
