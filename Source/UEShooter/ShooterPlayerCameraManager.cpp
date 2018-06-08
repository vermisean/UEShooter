// Copyright Sean Duffy 2018

#include "ShooterPlayerCameraManager.h"
#include "UEShooterCharacter.h"



AShooterPlayerCameraManager::AShooterPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NormalFOV = 90.0f;
	ADSFOV = 65.0f;
}

void AShooterPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	AUEShooterCharacter* MyPawn = PCOwner ? Cast<AUEShooterCharacter>(PCOwner->GetPawn()) : nullptr;

	if (MyPawn)
	{
		//const float ADSFOV = MyPawn->IsAiming() ? ADSFOV : NormalFOV;
		//DefaultFOV = FMath::FInterpTo(DefaultFOV, TargetFOV, DeltaTime, 20.0f);
		//SetFOV(DefaultFOV);
	}

	Super::UpdateCamera(DeltaTime);
}

