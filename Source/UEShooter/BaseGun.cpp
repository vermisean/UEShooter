// Copyright Sean Duffy 2018

#include "BaseGun.h"
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


ABaseGun::ABaseGun(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh"));
	Mesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	Mesh->bReceivesDecals = true;
	Mesh->CastShadow = true;
	Mesh->SetCollisionObjectType(ECC_WorldDynamic);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	RootComponent = Mesh;

	CurrentState = EGunState::Idle;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	StartAmmo = 32;
	MaxAmmo = 99;
	MaxAmmoPerClip = 16;
	NoAnimReloadDuration = 1.5f;
}

void ABaseGun::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TimeBetweenShots = 60.0f / ShotsPerMinute;
	CurrentAmmo = FMath::Min(StartAmmo, MaxAmmo);
	CurrentAmmoInClip = FMath::Min(MaxAmmoPerClip, StartAmmo);
}

void ABaseGun::BeginPlay()
{
	Super::BeginPlay();

}

FVector ABaseGun::GetAdjustedAim() const
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

FVector ABaseGun::GetCameraDamageStartLocation(const FVector& AimDir) const
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

FHitResult ABaseGun::WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const
{
	FCollisionQueryParams TraceParams(TEXT("WeaponTrace"), true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, COLLISION_WEAPON, TraceParams);

	return Hit;
}

void ABaseGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

USkeletalMeshComponent* ABaseGun::GetWeaponMesh() const
{
	return Mesh;
}

void ABaseGun::SetOwningPawn(AUEShooterCharacter* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}

class AUEShooterCharacter* ABaseGun::GetPawnOwner() const
{
	return MyPawn;
}

void ABaseGun::StartFire()
{

}

void ABaseGun::StopFire()
{

}

EGunState ABaseGun::GetCurrentState() const
{
	return CurrentState;
}

void ABaseGun::UseAmmo()
{
	CurrentAmmoInClip--;
	CurrentAmmo--;
}

FVector ABaseGun::GetMuzzleLocation() const
{
	return Muzzle->GetComponentLocation();
}

FVector ABaseGun::GetMuzzleDirection() const
{
	return Muzzle->GetComponentRotation().Vector();
}

UAudioComponent* ABaseGun::PlayWeaponSound(USoundCue* SoundToPlay)
{
	UAudioComponent* AudioComp = nullptr;
	if (SoundToPlay && MyPawn)
	{
		AudioComp = UGameplayStatics::SpawnSoundAttached(SoundToPlay, MyPawn->GetRootComponent());
	}

	return AudioComp;
}

float ABaseGun::PlayWeaponAnimation(UAnimMontage* Animation, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
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

void ABaseGun::StopWeaponAnimation(UAnimMontage* Animation)
{

}

void ABaseGun::ReloadGun()
{
	int32 ClipDelta = FMath::Min(MaxAmmoPerClip - CurrentAmmoInClip, CurrentAmmo - CurrentAmmoInClip);

	if (ClipDelta > 0)
	{
		CurrentAmmoInClip += ClipDelta;
	}
}

bool ABaseGun::CanReload()
{
	bool bHaveAmmo = (CurrentAmmoInClip < MaxAmmoPerClip) && ((CurrentAmmo - CurrentAmmoInClip) > 0);
	bool bStateOKToReload = ((CurrentState == EGunState::Idle) || (CurrentState == EGunState::Firing));
	return (bHaveAmmo && bStateOKToReload);
}

bool ABaseGun::CanFire() const
{
	bool bPawnCanFire = MyPawn && MyPawn->CanFire();
	bool bStateOK = CurrentState == EGunState::Idle || CurrentState == EGunState::Firing;
	return bPawnCanFire && bStateOK && !bPendingReload;
}

void ABaseGun::SimulateWeaponFire()
{
	if (MuzzleFX)
	{
		MuzzleParticles = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, Mesh, NAME_None, GetMuzzleLocation());
	}

	if (!bPlayingFireAnim)
	{
		PlayWeaponAnimation(FireAnim);
		bPlayingFireAnim = true;
	}

	PlayWeaponSound(FireSound);
}

void ABaseGun::StopSimulatingWeaponFire()
{
	if (bPlayingFireAnim)
	{
		StopWeaponAnimation(FireAnim);
		bPlayingFireAnim = false;
	}
}

void ABaseGun::StartReload()
{
	bPendingReload = true;
	DetermineGunState();

	float AnimDuration = PlayWeaponAnimation(ReloadAnim);
	if (AnimDuration <= 0.0f)
	{
		AnimDuration = NoAnimReloadDuration;
	}

	GetWorldTimerManager().SetTimer(TimerHandle_StopReload, this, &ABaseGun::StopReload, AnimDuration, false);
	GetWorldTimerManager().SetTimer(TimerHandle_ReloadWeapon, this, &ABaseGun::ReloadGun, FMath::Max(0.1f, AnimDuration - 0.1f), false);

	PlayWeaponSound(ReloadSound);
}

void ABaseGun::StopReload()
{
	if (CurrentState == EGunState::Reloading)
	{
		bPendingReload = false;
		DetermineGunState();
		StopWeaponAnimation(ReloadAnim);
	}
}

int32 ABaseGun::GiveAmmo(int32 AddAmount)
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

void ABaseGun::SetAmmoCount(int32 NewTotalAmount)
{
	CurrentAmmo = FMath::Min(MaxAmmo, NewTotalAmount);
	CurrentAmmoInClip = FMath::Min(MaxAmmoPerClip, CurrentAmmo);
}

int32 ABaseGun::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

int32 ABaseGun::GetCurrentAmmoInClip() const
{
	return CurrentAmmoInClip;
}

int32 ABaseGun::GetMaxAmmoPerClip() const
{
	return MaxAmmoPerClip;
}

int32 ABaseGun::GetMaxAmmo() const
{
	return MaxAmmo;
}

void ABaseGun::SetGunState(EGunState NewState)
{
	const EGunState PrevState = CurrentState;

	CurrentState = NewState;
}

void ABaseGun::DetermineGunState()
{
	EGunState NewState = EGunState::Idle;

	if (bPendingReload)
	{
		if (CanReload())
		{
			NewState = EGunState::Reloading;
		}
		else
		{
			NewState = CurrentState;
		}
	}
	else if (!bPendingReload && bWantsToFire && CanFire())
	{
		NewState = EGunState::Firing;
	}

	SetGunState(NewState);
}

void ABaseGun::HandleFiring()
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

		// Auto reload after firing last round
		if (CurrentAmmoInClip <= 0 && CanReload())
		{
			StartReload();
		}
	}

	if (MyPawn)
	{
		bRefiring = (CurrentState == EGunState::Firing && TimeBetweenShots > 0.0f);
		if (bRefiring)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &ABaseGun::HandleFiring, TimeBetweenShots, false);
		}
	}

	if (MyPawn)
	{
		MyPawn->MakeNoise(1.0f);
	}

	LastFireTime = GetWorld()->GetTimeSeconds();
}

