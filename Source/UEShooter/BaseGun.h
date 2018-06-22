// Copyright Sean Duffy 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGun.generated.h"

UENUM()
enum class EGunState
{
	Idle,
	Firing,
	Reloading
};

UCLASS(Abstract, Blueprintable)
class UESHOOTER_API ABaseGun : public AActor
{
	GENERATED_BODY()
	
	virtual void PostInitializeComponents() override;

	FTimerHandle TimerHandle_HandleFiring;

	UPROPERTY(EditDefaultsOnly)
	float ShotsPerMinute;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* Muzzle;

public:	

	UPROPERTY(EditDefaultsOnly)
	class AUEShooterCharacter* MyPawn;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* Mesh;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	USkeletalMeshComponent* GetWeaponMesh() const;

	// Set the weapon's owning pawn
	void SetOwningPawn(AUEShooterCharacter* NewOwner);

	// Get weapon's owning pawn
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	class AUEShooterCharacter* GetPawnOwner() const;

	void StartFire();

	void StopFire();

	EGunState GetCurrentState() const;

	void UseAmmo();

protected:

	ABaseGun(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	FVector GetAdjustedAim() const;

	FVector GetCameraDamageStartLocation(const FVector& AimDir) const;

	FHitResult WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const;

	virtual void FireWeapon() PURE_VIRTUAL(AShooterWeapon::FireWeapon, );

	FVector GetMuzzleLocation() const;

	FVector GetMuzzleDirection() const;

	class UAudioComponent* PlayWeaponSound(class USoundCue* SoundToPlay);

	float PlayWeaponAnimation(class UAnimMontage* Animation, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

	void StopWeaponAnimation(UAnimMontage* Animation);

	virtual void ReloadGun();

	bool CanReload();

	bool CanFire() const;

	virtual void SimulateWeaponFire();

	virtual void StopSimulatingWeaponFire();

	// Time to assign on reload when no animation is found
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
	class USoundCue* ReloadSound;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimMontage* ReloadAnim;

public:	

	virtual void Tick(float DeltaTime) override;

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

private:

	void SetGunState(EGunState NewState);

	void DetermineGunState();

	virtual void HandleFiring();

	bool bPlayingFireAnim;

	bool bWantsToFire;

	EGunState CurrentState;

	float LastFireTime;

	bool bRefiring;

	float TimeBetweenShots;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* OutOfAmmoSound;

	FTimerHandle TimerHandle_StopReload;

	FTimerHandle TimerHandle_ReloadWeapon;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* MuzzleFX;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* FireAnim;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystemComponent* MuzzleParticles;
	
};
