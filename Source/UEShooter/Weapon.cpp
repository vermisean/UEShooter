// Copyright Sean Duffy 2018

#include "Weapon.h"
#include "Components/AudioComponent.h"
#include "Runtime/Engine/Classes/sound/SoundCue.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"


// Sets default values
// AWeapon::AWeapon()
// {
//  	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
// 	PrimaryActorTick.bCanEverTick = true;
// 	PrimaryActorTick.TickGroup = TG_PrePhysics;
// 
// 	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
// 	Mesh->bReceivesDecals = true;
// 	Mesh->CastShadow = true;
// 	Mesh->SetCollisionObjectType(ECC_WorldDynamic);
// 	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// 	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
// 	Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
// 	RootComponent = Mesh;
// 	CurrentState = EWeaponState::Idle;
// 
// 	MuzzleAttachPoint = TEXT("MuzzleFlashSocket");
// 
// 	ShotsPerMinute = 999;
// 	StartAmmo = 999;
// 	MaxAmmo = 999;
// 	MaxAmmoPerClip = 999;
// 	NoAnimReloadDuration = 1.5f;
// }
// 
// // Called when the game starts or when spawned
// void AWeapon::BeginPlay()
// {
// 	Super::BeginPlay();
// 	
// 	TimeBetweenShots = 60.0f / ShotsPerMinute;
// 	CurrentAmmo = FMath::Min(StartAmmo, MaxAmmo);
// 	CurrentAmmoInClip = FMath::Min(MaxAmmoPerClip, StartAmmo);
// }
// 
// USkeletalMeshComponent* AWeapon::GetWeaponMesh() const
// {
// 	return Mesh;
// }
// 
// bool AWeapon::CanFire() const
// {
// 	return !bPendingReload;	///TODO also out of ammo, etc
// }
// 
// FVector AWeapon::GetAdjustedAim() const
// {
// 	FVector ZeroVec = FVector::ZeroVector;
// 	return ZeroVec;							//TODO implement this
// }
// 
// FHitResult AWeapon::WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const
// {
// 
// }
// 
// void AWeapon::FireWeapon() PURE_VIRTUAL(FireWeapon())
// {
// 
// }
// 
// void AWeapon::SetWeaponState(EWeaponState NewState)
// {
// 
// }
// 
// void AWeapon::DetermineWeaponState()
// {
// 
// }
// 
// void AWeapon::HandleFiring()
// {
// 
// }
// 
// void AWeapon::SimulateWeaponFire()
// {
// 
// }
// 
// void AWeapon::StopSimulateWeaponFire()
// {
// 
// }
// 
// FVector AWeapon::GetMuzzleLocation() const
// {
// 
// }
// 
// FVector AWeapon::GetMuzzleDirection() const
// {
// 
// }
// 
// UAudioComponent* AWeapon::PlayWeaponSound(USoundCue* SoundToPlay)
// {
// 
// }
// 
// float AWeapon::PlayWeaponAnim(UAnimMontage* Anim, float InPlayrate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
// {
// 
// }
// 
// void AWeapon::StopWeaponAnim(UAnimMontage* AnimToStop)
// {
// 
// }
// 
// void AWeapon::UseAmmo()
// {
// 
// }
// 
// void AWeapon::ReloadWeapon()
// {
// 
// }
// 
// bool AWeapon::CanReload()
// {
// 
// }
// 
// void AWeapon::StartReload()
// {
// 
// }
// 
// void AWeapon::StopSimulateReload()
// {
// 
// }
// 
// int32 AWeapon::GiveAmmo(int32 AddAmount)
// {
// 
// }
// 
// void AWeapon::SetAmmoCount(int32 NewTotalAmount)
// {
// 
// }
// 
// int32 AWeapon::GetCurrentAmmo() const
// {
// 
// }
// 
// int32 AWeapon::GetCurrentAmmoInClip() const
// {
// 
// }
// 
// int32 AWeapon::GetMaxAmmoPerClip() const
// {
// 
// }
// 
// int32 AWeapon::GetMaxAmmo() const
// {
// 
// }
// 
// // Called every frame
// void AWeapon::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// 
// }
// 
// void AWeapon::StartFire()
// {
// 
// }
// 
// void AWeapon::StopFire()
// {
// 
// }
// 
// EWeaponState AWeapon::GetCurrentState() const
// {
// 
// }
// 
