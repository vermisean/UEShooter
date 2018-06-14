// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UEShooterProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MonsterCharacter.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"

AUEShooterProjectile::AUEShooterProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AUEShooterProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	//RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3600.f;
	ProjectileMovement->MaxSpeed = 3600.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	// Hit particle effect
	HitSkinParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitSkinEffect"));
	HitSkinParticles->AttachToComponent(CollisionComp, FAttachmentTransformRules::KeepRelativeTransform);
	HitSkinParticles->bAutoActivate = false;

	// Hit particle effect
	HitParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitEffect"));
	HitParticles->AttachToComponent(CollisionComp, FAttachmentTransformRules::KeepRelativeTransform);
	HitParticles->bAutoActivate = false;



	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AUEShooterProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics object
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics() && !OtherActor->IsA(AMonsterCharacter::StaticClass()))
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 50.0f, GetActorLocation());
		
		SetRootComponent(HitParticles);
		HitParticles->Activate(true);

		CollisionComp->DestroyComponent();
	}
	// If a monster
	else if((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherActor->IsA(AMonsterCharacter::StaticClass()))
	{
		SetRootComponent(HitSkinParticles);
		HitSkinParticles->Activate(true);

		CollisionComp->DestroyComponent();
	}
	// if a static object
	else
	{
		SetRootComponent(HitParticles);
		HitParticles->Activate(true);

		CollisionComp->DestroyComponent();
	}

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AUEShooterProjectile::OnTimerExpire, DestroyDelay, false);
}

void AUEShooterProjectile::OnTimerExpire()
{
	Destroy();
}
