// Copyright Sean Duffy 2018

#pragma once

 #include "CoreMinimal.h"
 #include "GameFramework/Actor.h"
// #include "Weapon.generated.h"

// UENUM()
// enum class EWeaponState
// {
// 	Idle,
// 	Firing,
// 	Reloading
// 	
// };
// 
// UCLASS(ABSTRACT, Blueprintable)
// class UESHOOTER_API AWeapon : public AActor
// {
// 	GENERATED_BODY()
// 
// 
// 	FTimerHandle TimerHandle_HandleFiring;
// 
// 	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
// 	float ShotsPerMinute;
// 
// public:	
// 	// Sets default values for this actor's properties
// 	AWeapon();
// 
// 	// Called every frame
// 	virtual void Tick(float DeltaTime) override;
// 
// 	void StartFire();
// 
// 	void StopFire();
// 
// 	EWeaponState GetCurrentState() const;
// 
// 
// protected:
// 	// Called when the game starts or when spawned
// 	virtual void BeginPlay() override;
// 
// 	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
// 	class USkeletalMeshComponent* Mesh;
// 
// 	UFUNCTION(BlueprintCallable, Category = "Mesh")
// 	USkeletalMeshComponent* GetWeaponMesh() const;
// 
// 	bool CanFire() const;
// 
// 	FVector GetAdjustedAim() const;
// 
// 	FHitResult WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const;
// 
// 	virtual void FireWeapon() PURE_VIRTUAL(AWeapon::FireWeapon());
// 
// private:	
// 
// 	void SetWeaponState(EWeaponState NewState);
// 
// 	void DetermineWeaponState();
// 
// 	virtual void HandleFiring();
// 
// 	bool bWantsToFire;
// 
// 	EWeaponState CurrentState;
// 
// 	bool bRefiring;
// 
// 	float LastFireTime;
// 
// 	float TimeBetweenShots;
// 
// 	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
// 	class USoundCue* FireSound;
// 
// 	UPROPERTY(EditDefaultsOnly)
// 	class UParticleSystem* MuzzleFX;
// 
// 	UPROPERTY(EditDefaultsOnly)
// 	class UAnimMontage* FireAnim;
// 	
// 	UPROPERTY(EditDefaultsOnly)
// 	FName MuzzleAttachPoint;
// 
// 	bool bPlayingFireAnimation;
// 
// protected:
// 	virtual void SimulateWeaponFire();
// 
// 	virtual void StopSimulateWeaponFire();
// 
// 	FVector GetMuzzleLocation() const;
// 
// 	FVector GetMuzzleDirection() const;
// 
// 	class UAudioComponent* PlayWeaponSound(USoundCue* SoundToPlay);
// 
// 	float PlayWeaponAnim(UAnimMontage* Anim, float InPlayrate = 1.f, FName StartSectionName = NAME_None);
// 
// 	void StopWeaponAnim(UAnimMontage* AnimToStop);
// 
// 
// 	////// Ammo and Reloading
// private:
// 	UPROPERTY(EditDefaultsOnly, Category = "Sound")
// 	USoundCue* OutOfAmmoSound;
// 
// 	FTimerHandle TimerHandle_ReloadWeapon;
// 
// 	FTimerHandle TimerHandle_StopReload;
// 
// protected:
// 	UPROPERTY(EditDefaultsOnly, Category = "Animation")
// 	float NoAnimReloadDuration;
// 	
// 	bool bPendingReload;
// 
// 	void UseAmmo();
// 	
// 	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
// 	int32 CurrentAmmo;
// 
// 	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
// 	int32 CurrentAmmoInClip;
// 
// 	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
// 	int32 StartAmmo;
// 
// 	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
// 	int32 MaxAmmo;
// 
// 	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
// 	int32 MaxAmmoPerClip;
// 
// 	UPROPERTY(EditDefaultsOnly, Category = "Sound")
// 	USoundCue* ReloadSound;
// 
// 	UPROPERTY(EditDefaultsOnly, Category = "Animation")
// 	UAnimMontage* ReloadAnim;
// 
// 	virtual void ReloadWeapon();
// 
// 	bool CanReload();
// 
// public:
// 	virtual void StartReload();
// 
// 	virtual void StopSimulateReload();
// 
// 	int32 GiveAmmo(int32 AddAmount);
// 
// 	void SetAmmoCount(int32 NewTotalAmount);
// 
// 	UFUNCTION(BlueprintCallable, Category = "Ammo")
// 	int32 GetCurrentAmmo() const;
// 
// 	UFUNCTION(BlueprintCallable, Category = "Ammo")
// 	int32 GetCurrentAmmoInClip() const;
// 
// 	UFUNCTION(BlueprintCallable, Category = "Ammo")
// 	int32 GetMaxAmmoPerClip() const;
// 
// 	UFUNCTION(BlueprintCallable, Category = "Ammo")
// 	int32 GetMaxAmmo() const;
// 
// };
