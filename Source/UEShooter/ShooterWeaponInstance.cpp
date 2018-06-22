// Copyright Sean Duffy 2018

#include "ShooterWeaponInstance.h"
#include "Components/AudioComponent.h"
#include "Runtime/Engine/Classes/sound/SoundCue.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "MonsterCharacter.h"
#include "UEShooterCharacter.h"
#include "Kismet/GameplayStatics.h"


AShooterWeaponInstance::AShooterWeaponInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	HitDamage = 14;
	WeaponRange = 15000;

	AllowedViewDotHitDir = -1.0f;
	HitLeeway = 200.0f;
	MinimumProjectileSpawnDistance = 800;
	TracerRoundInterval = 3;
}

void AShooterWeaponInstance::SimulateInstantHit(const FVector& ImpactPoint)
{
	const FVector MuzzleOrigin = GetMuzzleLocation();

	// Adjust direction based on desired crosshair impact point and muzzle location
	const FVector AimDir = (ImpactPoint - MuzzleOrigin).GetSafeNormal();

	const FVector EndTrace = MuzzleOrigin + (AimDir * WeaponRange);
	const FHitResult Impact = WeaponTrace(MuzzleOrigin, EndTrace);

	if (Impact.bBlockingHit)
	{
		SpawnImpactEffects(Impact);
		SpawnTrailEffects(Impact.ImpactPoint);
	}
	else
	{
		SpawnTrailEffects(EndTrace);
	}
}

void AShooterWeaponInstance::SpawnImpactEffects(const FHitResult& Impact)
{

}

void AShooterWeaponInstance::SpawnTrailEffects(const FVector& EndPoint)
{
	// Keep local count for effects
	BulletsShotCount++;

	const FVector Origin = GetMuzzleLocation();
	FVector ShootDir = EndPoint - Origin;

	// Only spawn if a minimum distance is satisfied.
	if (ShootDir.Size() < MinimumProjectileSpawnDistance)
	{
		return;
	}

	if (BulletsShotCount % TracerRoundInterval == 0)
	{
		if (TracerFX)
		{
			ShootDir.Normalize();
			UGameplayStatics::SpawnEmitterAtLocation(this, TracerFX, Origin, ShootDir.Rotation());
		}
	}
	else
	{
		if (TrailFX)
		{
			UParticleSystemComponent* TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(this, TrailFX, Origin);
			if (TrailPSC)
			{
				TrailPSC->SetVectorParameter(TrailTargetParam, EndPoint);
			}
		}
	}
}

void AShooterWeaponInstance::FireWeapon()
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

	ProcessInstantHit(Impact, MuzzleOrigin, AdjustedAimDir);
}

void AShooterWeaponInstance::DealDamage(const FHitResult& Impact, const FVector& ShootDir)
{
	float ActualHitDamage = HitDamage;

	FPointDamageEvent PointDmg;
	PointDmg.HitInfo = Impact;
	PointDmg.ShotDirection = ShootDir;
	PointDmg.Damage = ActualHitDamage;

	Impact.GetActor()->TakeDamage(PointDmg.Damage, PointDmg, MyPawn->Controller, this);
}

bool AShooterWeaponInstance::ShouldDealDamage(AActor* TestActor) const
{
	if (TestActor)
	{
		return true;
	}

	return false;
}

void AShooterWeaponInstance::ProcessInstantHit(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDir)
{
	if (MyPawn)
	{
		if (Impact.GetActor())
		{
			// Notify the server of our local hit to validate and apply actual hit damage.
			SayNotifyHit(Impact, ShootDir);
		}
		else if (Impact.GetActor() == nullptr)
		{
			return;
		}
	}

	// Process a confirmed hit.
	ProcessInstantHitConfirmed(Impact, Origin, ShootDir);
}

void AShooterWeaponInstance::ProcessInstantHitConfirmed(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDir)
{
	// Handle damage
	if (ShouldDealDamage(Impact.GetActor()))
	{
		DealDamage(Impact, ShootDir);
	}

	HitImpactNotify = Impact.ImpactPoint;

	SimulateInstantHit(Impact.ImpactPoint);
}

void AShooterWeaponInstance::SayNotifyHit(const FHitResult Impact, FVector_NetQuantizeNormal ShootDir)
{
	if (Instigator && (Impact.GetActor() || Impact.bBlockingHit))
	{
		const FVector Origin = GetMuzzleLocation();
		const FVector ViewDir = (Impact.Location - Origin).GetSafeNormal();

		const float ViewDotHitDir = FVector::DotProduct(Instigator->GetViewRotation().Vector(), ViewDir);
		if (ViewDotHitDir > AllowedViewDotHitDir)
		{
			// TODO: Check for weapon state

			if (Impact.GetActor() == nullptr)
			{
				if (Impact.bBlockingHit)
				{
					ProcessInstantHitConfirmed(Impact, Origin, ShootDir);
				}
			}
			// Assume it told the truth about static things because we don't move and the hit
			// usually doesn't have significant gameplay implications
			else if (Impact.GetActor()->IsRootComponentStatic() || Impact.GetActor()->IsRootComponentStationary())
			{
				ProcessInstantHitConfirmed(Impact, Origin, ShootDir);
			}
			else
			{
				const FBox HitBox = Impact.GetActor()->GetComponentsBoundingBox();

				FVector BoxExtent = 0.5 * (HitBox.Max - HitBox.Min);
				BoxExtent *= HitLeeway;

				BoxExtent.X = FMath::Max(20.0f, BoxExtent.X);
				BoxExtent.Y = FMath::Max(20.0f, BoxExtent.Y);
				BoxExtent.Z = FMath::Max(20.0f, BoxExtent.Z);

				const FVector BoxCenter = (HitBox.Min + HitBox.Max) * 0.5;

				// If we are within client tolerance
				if (FMath::Abs(Impact.Location.Z - BoxCenter.Z) < BoxExtent.Z &&
					FMath::Abs(Impact.Location.X - BoxCenter.X) < BoxExtent.X &&
					FMath::Abs(Impact.Location.Y - BoxCenter.Y) < BoxExtent.Y)
				{
					ProcessInstantHitConfirmed(Impact, Origin, ShootDir);
				}
			}
		}
	}
}
