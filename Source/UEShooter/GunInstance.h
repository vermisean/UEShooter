// Copyright Sean Duffy 2018

#pragma once

#include "CoreMinimal.h"
#include "BaseGun.h"
#include "GunInstance.generated.h"

/**
 * Implementable Gun class for pistols, shotguns, etc
 */
UCLASS()
class UESHOOTER_API AGunInstance : public ABaseGun
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float HitDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float WeaponRange;

protected:

	AGunInstance(const FObjectInitializer& ObjectInitializer);

	// Damage Processing  
	virtual void FireWeapon() override;

	void DealDamage(const FHitResult& Impact, const FVector& ShootDir);

	bool ShouldDealDamage(AActor* TestActor) const;

private:

	// Keeps track of number of shots fired
	int32 BulletsShotCount;
	
};
