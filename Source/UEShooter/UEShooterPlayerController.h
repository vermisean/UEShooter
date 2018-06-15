// Copyright Sean Duffy 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UEShooterPlayerController.generated.h"

/**
 * Sets default camera class and will eventually implement respawning/spectating flags
 */
UCLASS()
class UESHOOTER_API AUEShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

	AUEShooterPlayerController(const class FObjectInitializer& ObjectInitializer);
	
	
};
