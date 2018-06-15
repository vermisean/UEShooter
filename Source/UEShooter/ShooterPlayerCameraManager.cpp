// Copyright Sean Duffy 2018

#include "ShooterPlayerCameraManager.h"
#include "UEShooterCharacter.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"



AShooterPlayerCameraManager::AShooterPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NormalFOV = 90.0f;
	ADSFOV = 65.0f;

	ViewPitchMin = -80.0f;
	ViewPitchMax = 87.0f;
	bAlwaysApplyModifiers = true;
}

void AShooterPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	AUEShooterCharacter* MyPawn = PCOwner ? Cast<AUEShooterCharacter>(PCOwner->GetPawn()) : nullptr;

	if (MyPawn)
	{
		const float TargetingFOV = MyPawn->IsTargeting() ? ADSFOV : NormalFOV;
		DefaultFOV = FMath::FInterpTo(DefaultFOV, TargetingFOV, DeltaTime, 20.0f);
		SetFOV(DefaultFOV);
	}

	Super::UpdateCamera(DeltaTime);
}

