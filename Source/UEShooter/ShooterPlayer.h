// Copyright Sean Duffy 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterPlayer.generated.h"

UCLASS()
class UESHOOTER_API AShooterPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// Movement
	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	// Jumping
	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	// Firing
	UFUNCTION()
	void Fire();

	// Gun muzzle's offset from camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector MuzzleOffset;

	// Projectile to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AUEShooterProjectile> ProjectileClass;

	// Camera
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* FPSCameraComponent = nullptr;

	// Mesh
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* FPSMesh = nullptr;
	
};
