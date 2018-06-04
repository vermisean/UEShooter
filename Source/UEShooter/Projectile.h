// Copyright Sean Duffy 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class UESHOOTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// 	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
// 	class USphereComponent* CollisionComponent = nullptr;
// 
// 	UPROPERTY(VisibleAnywhere, Category = "Movement")
// 	class UProjectileMovementComponent* ProjectileMovementComponent = nullptr;
// 
// 	UPROPERTY(VisibleAnywhere, Category = "Movement")
// 	float ProjectileInitialSpeed = 3000.0f;
// 
// 	UPROPERTY(VisibleAnywhere, Category = "Movement")
// 	float ProjectileMaxSpeed = 3000.0f;
// 
// 	void FireInDirection(const FVector& ShootDirection);
	
};
