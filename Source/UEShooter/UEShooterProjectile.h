// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "UEShooterProjectile.generated.h"

UCLASS(config=Game)
class AUEShooterProjectile : public AActor
{
	GENERATED_BODY()


	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	void DealDamage(const FHitResult& Impact, const FVector& ShootDir);

public:
	AUEShooterProjectile();

	UPROPERTY(EditDefaultsOnly)
	class AUEShooterCharacter* MyPawn;

	/* Set the weapon's owning pawn */
	void SetOwningPawn(AUEShooterCharacter* NewOwner);

	/* Get pawn owner */
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	class AUEShooterCharacter* GetPawnOwner() const;

	// Damage amount
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float HitDamage;

	// Particle Effect to play each time we hit non humans
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UParticleSystemComponent* HitParticles;

	// Particle effect to play each time we hit humans
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UParticleSystemComponent* HitSkinParticles;

	// Timer before destruction
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float DestroyDelay = 5.0f;

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Called when timer expires, destroying the projectile
	void OnTimerExpire();

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
	bool ShouldDealDamage(AActor* TestActor) const;


};

