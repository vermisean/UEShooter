// Copyright Sean Duffy 2018

#include "UEShooterPlayerController.h"
#include "ShooterPlayerCameraManager.h"




AUEShooterPlayerController::AUEShooterPlayerController(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AShooterPlayerCameraManager::StaticClass();
}
