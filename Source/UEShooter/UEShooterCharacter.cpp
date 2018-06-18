// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UEShooterCharacter.h"
#include "UEShooterProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/GameFramework/PawnMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"


DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AUEShooterCharacter

AUEShooterCharacter::AUEShooterCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	//FP_Gun->SetupAttachment(Mesh1P, TEXT("WeaponSocket"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	FireParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FlashLocation"));
	FireParticles->AttachToComponent(FP_MuzzleLocation, FAttachmentTransformRules::KeepRelativeTransform);
	FireParticles->bAutoActivate = false;

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	TargetingSpeedModifier = 0.5f;
	SprintingSpeedModifier = 2.5f;
}

void AUEShooterCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("WeaponSocket"));
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUEShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AUEShooterCharacter::OnFire);

	// Bind sprint event
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AUEShooterCharacter::OnStartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AUEShooterCharacter::OnStopSprinting);

	// Bind aiming event
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AUEShooterCharacter::OnStartTargeting);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AUEShooterCharacter::OnEndTargeting);

	// Bind reload event
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AUEShooterCharacter::OnReload);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AUEShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUEShooterCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUEShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUEShooterCharacter::LookUpAtRate);

}

bool AUEShooterCharacter::CanFire() const
{
	return IsAlive();
}

bool AUEShooterCharacter::CanReload() const
{
	return IsAlive();
}

bool AUEShooterCharacter::IsFiring() const
{
	// TODO set weapon state firing - TODO set up weapon states

	return false;
}

void AUEShooterCharacter::OnStartSprinting()
{
	SetSprinting(true);

	
// 	float MaxSpeed = GetMovementComponent()->GetMaxSpeed();
// 
// 	if (IsSprinting())
// 	{
// 		Max
// 	}
}

void AUEShooterCharacter::OnStopSprinting()
{
	SetSprinting(false);
}

void AUEShooterCharacter::SetSprinting(bool NewSprinting)
{
	bWantsToRun = NewSprinting;

// 	if (bWantsToRun)
// 	{
// 		StopWeaponFire();
// 	}

	Super::SetSprinting(NewSprinting);
}

bool AUEShooterCharacter::IsSprinting() const
{
	if (!GetCharacterMovement())
		return false;

	return bWantsToRun && !IsTargeting() && !GetVelocity().IsZero() && (GetVelocity().GetSafeNormal2D() | GetActorRotation().Vector()) > 0.1; 
}

float AUEShooterCharacter::GetSprintingSpeedModifier() const
{
	return SprintingSpeedModifier;
}

void AUEShooterCharacter::OnStartTargeting()
{
	SetTargeting(true);
}

void AUEShooterCharacter::OnEndTargeting()
{
	SetTargeting(false);
}

void AUEShooterCharacter::SetTargeting(bool NewTargeting)
{
	bIsTargeting = NewTargeting;
}

bool AUEShooterCharacter::IsTargeting() const
{
	return bIsTargeting;
}

float AUEShooterCharacter::GetTargetingSpeedModifier() const
{
	return TargetingSpeedModifier;
}

FRotator AUEShooterCharacter::GetAimOffsets() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}

void AUEShooterCharacter::OnReload()
{
	// CurrentWeapon->StartReload();
}

void AUEShooterCharacter::OnStartFire()
{
 	if (IsSprinting())
 	{
 		SetSprinting(false);
 	}

	StartWeaponFire();
}

void AUEShooterCharacter::OnStopFire()
{
	StopWeaponFire();
}

void AUEShooterCharacter::StartWeaponFire()
{
	if (BWantsToFire)
	{
		BWantsToFire = true;
// 		if (CurrentWeapon)
// 		{
// 			CurrentWeapon->StartFire();
// 		}
	}
}

void AUEShooterCharacter::StopWeaponFire()
{
	if (BWantsToFire)
	{
		BWantsToFire = false;
		// 		if (CurrentWeapon)
		// 		{
		// 			CurrentWeapon->StopFire();
		// 		}
	}
}

float AUEShooterCharacter::GetLastNoiseLoudness()
{
	return LastNoiseLoudness;
}

float AUEShooterCharacter::GetLastMakeNoiseTime()
{
	return LastMakeNoiseTime;
}

void AUEShooterCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<AUEShooterProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play particle effect
	if (FireParticles != NULL)
	{
		FireParticles->Activate(true);
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL && FireADSAnimation != NULL)
	{
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			if (IsTargeting())
			{
				AnimInstance->Montage_Play(FireADSAnimation, 1.f);

			}
			else
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}
}


void AUEShooterCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AUEShooterCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AUEShooterCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUEShooterCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

