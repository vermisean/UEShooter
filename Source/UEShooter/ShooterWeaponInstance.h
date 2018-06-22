// Copyright Sean Duffy 2018

#pragma once

#include "CoreMinimal.h"
#include "ShooterWeapon.h"
#include "ShooterWeaponInstance.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UESHOOTER_API AShooterWeaponInstance : public AShooterWeapon
{
	GENERATED_BODY()
	
private:
	void SimulateInstantHit(const FVector& ImpactPoint);

	void SpawnImpactEffects(const FHitResult& Impact);

	void SpawnTrailEffects(const FVector& EndPoint);

	// Particle FX played when a surface is hit.
// 	UPROPERTY(EditDefaultsOnly)
// 	TSubclassOf<class ASImpactEffect> ImpactTemplate;

	UPROPERTY(EditDefaultsOnly)
	FName TrailTargetParam;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* TrailFX;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* TracerFX;

	// Minimum firing distance before spawning tracers or trails.
	UPROPERTY(EditDefaultsOnly)
	float MinimumProjectileSpawnDistance;

	UPROPERTY(EditDefaultsOnly)
	int32 TracerRoundInterval;

	// Keeps track of number of shots fired
	int32 BulletsShotCount;

protected:

	AShooterWeaponInstance(const FObjectInitializer& ObjectInitializer);

	// Damage Processing  
	virtual void FireWeapon() override;

	void DealDamage(const FHitResult& Impact, const FVector& ShootDir);

	bool ShouldDealDamage(AActor* TestActor) const;

	void ProcessInstantHit(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDir);

	void ProcessInstantHitConfirmed(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDir);

	UFUNCTION()
	void SayNotifyHit(const FHitResult Impact, FVector_NetQuantizeNormal ShootDir);

	UPROPERTY()
	FVector HitImpactNotify;

	// Weapon Config
	UPROPERTY(EditDefaultsOnly)
	float HitDamage;

	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly)
	float WeaponRange;

	// Hit verification: threshold for dot product between view direction and hit direction
	UPROPERTY(EditDefaultsOnly)
	float AllowedViewDotHitDir;

	// Hit verification: scale for bounding box of hit actor 
	UPROPERTY(EditDefaultsOnly)
	float HitLeeway;
};
