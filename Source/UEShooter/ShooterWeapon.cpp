// Copyright Sean Duffy 2018

#include "ShooterWeapon.h"
#include "UEShooterCharacter.h"
#include "Types.h"
#include "UEShooterPlayerController.h"
#include "Components/AudioComponent.h"
#include "Runtime/Engine/Classes/sound/SoundCue.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "UEShooter.h"


AShooterWeapon::AShooterWeapon(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	Mesh = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh3P"));
	Mesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	Mesh->bReceivesDecals = true;
	Mesh->CastShadow = true;
	Mesh->SetCollisionObjectType(ECC_WorldDynamic);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	RootComponent = Mesh;

	CurrentState = EWeaponState::Idle;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	MuzzleAttachPoint = TEXT("MuzzleFlashSocket");

	ShotsPerMinute = 60;
	StartAmmo = 32;
	MaxAmmo = 99;
	MaxAmmoPerClip = 16;
	NoAnimReloadDuration = 1.5f;
}


void AShooterWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TimeBetweenShots = 60.0f / ShotsPerMinute;
	CurrentAmmo = FMath::Min(StartAmmo, MaxAmmo);
	CurrentAmmoInClip = FMath::Min(MaxAmmoPerClip, StartAmmo);
}

USkeletalMeshComponent* AShooterWeapon::GetWeaponMesh() const
{
	return Mesh;
}

void AShooterWeapon::SetOwningPawn(AUEShooterCharacter* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}

class AUEShooterCharacter* AShooterWeapon::GetPawnOwner() const
{
	return MyPawn;
}

void AShooterWeapon::StartFire()
{
	if (!bWantsToFire)
	{
		bWantsToFire = true;
		DetermineWeaponState();
	}
}

void AShooterWeapon::StopFire()
{
	if (bWantsToFire)
	{
		bWantsToFire = false;
		DetermineWeaponState();
	}
}

EWeaponState AShooterWeapon::GetCurrentState() const
{
	return CurrentState;
}

void AShooterWeapon::UseAmmo()
{
	CurrentAmmoInClip--;
	CurrentAmmo--;
}

bool AShooterWeapon::CanFire() const
{
	bool bPawnCanFire = MyPawn && MyPawn->CanFire();
	bool bStateOK = CurrentState == EWeaponState::Idle || CurrentState == EWeaponState::Firing;
	return bPawnCanFire && bStateOK && !bPendingReload;
}

void AShooterWeapon::SimulateWeaponFire()
{
	if (MuzzleFX)
	{
		MuzzleParticles = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, Mesh, MuzzleAttachPoint);
	}

	if (!bPlayingFireAnim)
	{
		PlayWeaponAnimation(FireAnim);
		bPlayingFireAnim = true;
	}

	PlayWeaponSound(FireSound);
}

void AShooterWeapon::StopSimulatingWeaponFire()
{
	if (bPlayingFireAnim)
	{
		StopWeaponAnimation(FireAnim);
		bPlayingFireAnim = false;
	}
}

FVector AShooterWeapon::GetAdjustedAim() const
{
	AUEShooterPlayerController* const PC = Instigator ? Cast<AUEShooterPlayerController>(Instigator->Controller) : nullptr;
	FVector FinalAim = FVector::ZeroVector;

	if (PC)
	{
		FVector CamLoc;
		FRotator CamRot;
		PC->GetPlayerViewPoint(CamLoc, CamRot);

		FinalAim = CamRot.Vector();
	}
	else if (Instigator)
	{
		FinalAim = Instigator->GetBaseAimRotation().Vector();
	}

	return FinalAim;
}

FVector AShooterWeapon::GetCameraDamageStartLocation(const FVector& AimDir) const
{
	AUEShooterPlayerController* PC = MyPawn ? Cast<AUEShooterPlayerController>(MyPawn->Controller) : nullptr;
	FVector OutStartTrace = FVector::ZeroVector;

	if (PC)
	{
		FRotator DummyRot;
		PC->GetPlayerViewPoint(OutStartTrace, DummyRot);

		// Adjust trace so there is nothing blocking the ray between the camera and the pawn, and calculate distance from adjusted start
		OutStartTrace = OutStartTrace + AimDir * (FVector::DotProduct((Instigator->GetActorLocation() - OutStartTrace), AimDir));
	}

	return OutStartTrace;
}

FHitResult AShooterWeapon::WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const
{
	FCollisionQueryParams TraceParams(TEXT("WeaponTrace"), true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, COLLISION_WEAPON, TraceParams);

	return Hit;
}

void AShooterWeapon::SetWeaponState(EWeaponState NewState)
{
	const EWeaponState PrevState = CurrentState;

	CurrentState = NewState;
}

void AShooterWeapon::DetermineWeaponState()
{
	EWeaponState NewState = EWeaponState::Idle;

	if (bPendingReload)
	{
		if (CanReload())
		{
			NewState = EWeaponState::Reloading;
		}
		else
		{
			NewState = CurrentState;
		}
	}
	else if (!bPendingReload && bWantsToFire && CanFire())
	{
		NewState = EWeaponState::Firing;
	}

	SetWeaponState(NewState);
}

void AShooterWeapon::HandleFiring()
{
	if (CurrentAmmoInClip > 0 && CanFire())
	{
		SimulateWeaponFire();

		if (MyPawn)
		{
			FireWeapon();

			UseAmmo();
		}
	}
	else if (CanReload())
	{
		StartReload();
	}
	else if (MyPawn)
	{
		if (GetCurrentAmmo() == 0 && !bRefiring)
		{
			PlayWeaponSound(OutOfAmmoSound);
		}

		// Reload after firing last round
		if (CurrentAmmoInClip <= 0 && CanReload())
		{
			StartReload();
		}
	}

	if (MyPawn)
	{
		bRefiring = (CurrentState == EWeaponState::Firing && TimeBetweenShots > 0.0f);
		if (bRefiring)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &AShooterWeapon::HandleFiring, TimeBetweenShots, false);
		}
	}

// 	if (MyPawn)
// 	{
// 		MyPawn->MakePawnNoise(1.0f);
// 	}

	LastFireTime = GetWorld()->GetTimeSeconds();
}

FVector AShooterWeapon::GetMuzzleLocation() const
{
	return Mesh->GetSocketLocation(MuzzleAttachPoint);
}

FVector AShooterWeapon::GetMuzzleDirection() const
{
	return Mesh->GetSocketRotation(MuzzleAttachPoint).Vector();
}

class UAudioComponent* AShooterWeapon::PlayWeaponSound(USoundCue* SoundToPlay)
{
	UAudioComponent* AudioComp = nullptr;
	if (SoundToPlay && MyPawn)
	{
		AudioComp = UGameplayStatics::SpawnSoundAttached(SoundToPlay, MyPawn->GetRootComponent());
	}

	return AudioComp;
}

float AShooterWeapon::PlayWeaponAnimation(UAnimMontage* Animation, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	float Duration = 0.0f;
	if (MyPawn)
	{
		if (Animation)
		{
			Duration = MyPawn->PlayAnimMontage(Animation, InPlayRate, StartSectionName);
		}
	}

	return Duration;
}

void AShooterWeapon::StopWeaponAnimation(UAnimMontage* Animation)
{

}

void AShooterWeapon::ReloadWeapon()
{
	int32 ClipDelta = FMath::Min(MaxAmmoPerClip - CurrentAmmoInClip, CurrentAmmo - CurrentAmmoInClip);

	if (ClipDelta > 0)
	{
		CurrentAmmoInClip += ClipDelta;
	}
}

bool AShooterWeapon::CanReload()
{
	bool bHaveAmmo = (CurrentAmmoInClip < MaxAmmoPerClip) && ((CurrentAmmo - CurrentAmmoInClip) > 0);
	bool bStateOKToReload = ((CurrentState == EWeaponState::Idle) || (CurrentState == EWeaponState::Firing));
	return (bHaveAmmo && bStateOKToReload);
}

void AShooterWeapon::StartReload()
{
	bPendingReload = true;
	DetermineWeaponState();

	float AnimDuration = PlayWeaponAnimation(ReloadAnim);
	if (AnimDuration <= 0.0f)
	{
		AnimDuration = NoAnimReloadDuration;
	}

	GetWorldTimerManager().SetTimer(TimerHandle_StopReload, this, &AShooterWeapon::StopReload, AnimDuration, false);
	GetWorldTimerManager().SetTimer(TimerHandle_ReloadWeapon, this, &AShooterWeapon::ReloadWeapon, FMath::Max(0.1f, AnimDuration - 0.1f), false);

	PlayWeaponSound(ReloadSound);

}

void AShooterWeapon::StopReload()
{
	if (CurrentState == EWeaponState::Reloading)
	{
		bPendingReload = false;
		DetermineWeaponState();
		StopWeaponAnimation(ReloadAnim);
	}
}

int32 AShooterWeapon::GiveAmmo(int32 AddAmount)
{
	const int32 MissingAmmo = FMath::Max(0, MaxAmmo - CurrentAmmo);
	AddAmount = FMath::Min(AddAmount, MissingAmmo);
	CurrentAmmo += AddAmount;

	if (GetCurrentAmmoInClip() <= 0 && CanReload())
	{
		StartReload();
	}

	// Return the unused ammo when weapon is filled
	return FMath::Max(0, AddAmount - MissingAmmo);
}

void AShooterWeapon::SetAmmoCount(int32 NewTotalAmount)
{
	CurrentAmmo = FMath::Min(MaxAmmo, NewTotalAmount);
	CurrentAmmoInClip = FMath::Min(MaxAmmoPerClip, CurrentAmmo);
}

int32 AShooterWeapon::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

int32 AShooterWeapon::GetCurrentAmmoInClip() const
{
	return CurrentAmmoInClip;
}

int32 AShooterWeapon::GetMaxAmmoPerClip() const
{
	return MaxAmmoPerClip;
}

int32 AShooterWeapon::GetMaxAmmo() const
{
	return MaxAmmo;
}
