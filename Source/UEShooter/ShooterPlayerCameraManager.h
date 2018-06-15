// Copyright Sean Duffy 2018

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "ShooterPlayerCameraManager.generated.h"

/**
 * Manages the update of FOV when aiming
 */
UCLASS()
class UESHOOTER_API AShooterPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_UCLASS_BODY()

	virtual void UpdateCamera(float DeltaTime) override;

	// Default FOV
	float NormalFOV;
	
	// Aiming Down Sight FOV
	float ADSFOV;
};
