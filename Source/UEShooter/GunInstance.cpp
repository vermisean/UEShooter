// Copyright Sean Duffy 2018

#include "GunInstance.h"
#include "Components/AudioComponent.h"
#include "Runtime/Engine/Classes/sound/SoundCue.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "MonsterCharacter.h"
#include "UEShooterCharacter.h"
#include "Kismet/GameplayStatics.h"



AGunInstance::AGunInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	HitDamage = 14;
	WeaponRange = 15000;


}

void AGunInstance::FireWeapon()
{
	const FVector AimDir = GetAdjustedAim();
	const FVector CameraPos = GetCameraDamageStartLocation(AimDir);
	const FVector EndPos = CameraPos + (AimDir * WeaponRange);

	// Check for impact by tracing from the camera position
	FHitResult Impact = WeaponTrace(CameraPos, EndPos);

	const FVector MuzzleOrigin = GetMuzzleLocation();

	FVector AdjustedAimDir = AimDir;
	if (Impact.bBlockingHit)
	{
		// Adjust the shoot direction to hit at the crosshair.
		AdjustedAimDir = (Impact.ImpactPoint - MuzzleOrigin).GetSafeNormal();

		// Re-trace with the new aim direction coming out of the weapon muzzle
		Impact = WeaponTrace(MuzzleOrigin, MuzzleOrigin + (AdjustedAimDir * WeaponRange));
	}
	else
	{
		// Use the maximum distance as the adjust direction
		Impact.ImpactPoint = FVector_NetQuantize(EndPos);
	}

	if (ShouldDealDamage(Impact.GetActor()))
	{
		//DealDamage(Impact, ShootDir);
	}

	//ProcessInstantHit(Impact, MuzzleOrigin, AdjustedAimDir);
}

void AGunInstance::DealDamage(const FHitResult& Impact, const FVector& ShootDir)
{
	float ActualHitDamage = HitDamage;

	FPointDamageEvent PointDmg;
	PointDmg.HitInfo = Impact;
	PointDmg.ShotDirection = ShootDir;
	PointDmg.Damage = ActualHitDamage;

	Impact.GetActor()->TakeDamage(PointDmg.Damage, PointDmg, MyPawn->Controller, this);
}

bool AGunInstance::ShouldDealDamage(AActor* TestActor) const
{
	if (TestActor)
	{
		return true;
	}

	return false;
}
