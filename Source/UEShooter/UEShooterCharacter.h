// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "UEShooterCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AUEShooterCharacter : public ABaseCharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	float LastNoiseLoudness;

	float LastMakeNoiseTime;

	virtual void Tick(float DeltaSeconds) override;

public:
	AUEShooterCharacter(const class FObjectInitializer& ObjectInitializer);

	// Damage Camera Shake Asset
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> DamageShake;

	// Gun Camera Shake Asset
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> GunShake;

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	// Gun class
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	//TSubclassOf<class AShooterWeaponInstance> GunClass;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AUEShooterProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire when NOT Aiming Down Sights */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	/** AnimMontage to play each time we fire while Aiming Down Sights*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireADSAnimation;

	/** Particle Effect to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UParticleSystemComponent* FireParticles;

	UFUNCTION(BlueprintCallable, Category = "AI")
	float GetLastNoiseLoudness();

	UFUNCTION(BlueprintCallable, Category = "AI")
	float GetLastMakeNoiseTime();

protected:
	
	/** Fires a projectile. */
	void OnFire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);
	
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	bool CanFire() const;

	bool CanReload() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool IsFiring() const;

	void OnStartSprinting();

	void OnStopSprinting();

	virtual void SetSprinting(bool NewSprinting);

	UFUNCTION(BlueprintCallable, Category = Movement)
	bool IsSprinting() const;

	float GetSprintingSpeedModifier() const;

	void OnStartTargeting();

	void OnEndTargeting();

	void SetTargeting(bool NewTargeting);

	/* Is player aiming down sights */
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	bool IsTargeting() const;

	float GetTargetingSpeedModifier() const;

	/* Retrieve Pitch/Yaw from current camera */
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	FRotator GetAimOffsets() const;

	UPROPERTY(BlueprintReadWrite)
	bool bIsTargeting;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	float TargetingSpeedModifier;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

private:
	bool BWantsToFire;

	void OnReload();

	void OnStartFire();

	void OnStopFire();

	void StartWeaponFire();
	
	void StopWeaponFire();
};

