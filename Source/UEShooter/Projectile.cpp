// Copyright Sean Duffy 2018

#include "Projectile.h"
#include "Classes/Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up collision component
// 	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
// 	CollisionComponent->InitSphereRadius(15.0f);
// 	CollisionComponent->SetupAttachment(RootComponent);
// 
// 	ProjectileMovementComponent->CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
// 	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
// 	ProjectileMovementComponent->InitialSpeed = ProjectileInitialSpeed;
// 	ProjectileMovementComponent->MaxSpeed = ProjectileMaxSpeed;
// 	ProjectileMovementComponent->bRotationFollowsVelocity = true;
// 	ProjectileMovementComponent->bShouldBounce = false;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Initializes the projectile's velocity in the direction of the shot
// void AProjectile::FireInDirection(const FVector& ShootDirection)
// {
// 	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileInitialSpeed;
// }

