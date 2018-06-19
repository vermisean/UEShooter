// Copyright Sean Duffy 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterWeapon.generated.h"


UENUM()
enum class EWeaponState
{
	Idle,
	Firing,
	Reloading
};


UCLASS(ABSTRACT, Blueprintable)
class UESHOOTER_API AShooterWeapon : public AActor
{
	GENERATED_BODY()
	
	virtual void PostInitializeComponents() override;

	FTimerHandle TimerHandle_HandleFiring;

	UPROPERTY(EditDefaultsOnly)
	float ShotsPerMinute;

protected:

	AShooterWeapon(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly)
	class AUEShooterCharacter* MyPawn;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* Mesh;

public:
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	USkeletalMeshComponent* GetWeaponMesh() const;

	/* Set the weapon's owning pawn */
	void SetOwningPawn(AUEShooterCharacter* NewOwner);

	/* Get pawn owner */
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	class AUEShooterCharacter* GetPawnOwner() const;

	// Firing & Damage Handling
public:

	void StartFire();

	void StopFire();

	EWeaponState GetCurrentState() const;

	void UseAmmo();

protected:

	bool CanFire() const;

	virtual void SimulateWeaponFire();

	virtual void StopSimulatingWeaponFire();

	FVector GetAdjustedAim() const;

	FVector GetCameraDamageStartLocation(const FVector& AimDir) const;

	FHitResult WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const;

	virtual void FireWeapon() PURE_VIRTUAL(ASWeapon::FireWeapon, );

private:

	void SetWeaponState(EWeaponState NewState);

	void DetermineWeaponState();

	virtual void HandleFiring();

	bool bWantsToFire;

	EWeaponState CurrentState;

	bool bRefiring;

	float LastFireTime;

	// Time between shots for repeating fire 
	float TimeBetweenShots;

	// Simulation & FX  
private:
	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	class USoundCue* FireSound;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* MuzzleFX;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* FireAnim;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystemComponent* MuzzleParticles;

	UPROPERTY(EditDefaultsOnly)
	FName MuzzleAttachPoint;

	bool bPlayingFireAnim;

protected:
	FVector GetMuzzleLocation() const;

	FVector GetMuzzleDirection() const;

	class UAudioComponent* PlayWeaponSound(USoundCue* SoundToPlay);

	float PlayWeaponAnimation(UAnimMontage* Animation, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

	void StopWeaponAnimation(UAnimMontage* Animation);


	// Ammo & Reloading 
private:

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* OutOfAmmoSound;

	FTimerHandle TimerHandle_ReloadWeapon;

	FTimerHandle TimerHandle_StopReload;

protected:

	/* Time to assign on reload when no animation is found */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	float NoAnimReloadDuration;

	UPROPERTY(EditDefaultsOnly)
	bool bPendingReload;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentAmmo;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentAmmoInClip;

	// Weapon ammo on spawn 
	UPROPERTY(EditDefaultsOnly)
	int32 StartAmmo;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxAmmoPerClip;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* ReloadSound;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* ReloadAnim;

	virtual void ReloadWeapon();

	bool CanReload();

public:

	virtual void StartReload();

	virtual void StopReload();

	// Give ammo to weapon and return the amount that was not used
	int32 GiveAmmo(int32 AddAmount);

	// Set a new total amount of ammo of weapon 
	void SetAmmoCount(int32 NewTotalAmount);

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	int32 GetCurrentAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	int32 GetCurrentAmmoInClip() const;

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	int32 GetMaxAmmoPerClip() const;

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	int32 GetMaxAmmo() const;
};
