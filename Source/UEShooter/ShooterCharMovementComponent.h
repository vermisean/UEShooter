// Copyright Sean Duffy 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterCharMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class UESHOOTER_API UShooterCharMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	virtual float GetMaxSpeed() const override;
	
	
};
