// Copyright Sean Duffy 2018

#include "ShooterCharMovementComponent.h"
#include "BaseCharacter.h"
#include "UEShooterCharacter.h"


float UShooterCharMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	const ABaseCharacter* CharOwner = Cast<ABaseCharacter>(PawnOwner);
	if (CharOwner)
	{
		// Slow down during targeting or crouching
// 		if (CharOwner->IsTargeting())
// 		{
// 			MaxSpeed *= CharOwner->GetTargetingSpeedModifier();
// 		}
		if (CharOwner->IsSprinting())
		{
			MaxSpeed *= CharOwner->GetSprintingSpeedModifier();
		}
	}

	return MaxSpeed;
}

