// Copyright Sean Duffy 2018

#include "MonsterCharacter.h"
#include "UEShooterCharacter.h"
#include "MonsterAIController.h"
#include "AIWaypoint.h"
#include "BaseCharacter.h"
#include "Types.h"
#include "UEShooterProjectile.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Classes/GameFramework/PawnMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "UEShooter.h"



AMonsterCharacter::AMonsterCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PawnSenseComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSenseComp"));
	PawnSenseComp->SetPeripheralVisionAngle(60.0f);
	PawnSenseComp->SightRadius = 2000;
	PawnSenseComp->HearingThreshold = 600;
	PawnSenseComp->LOSHearingThreshold = 1200;

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f, false);
	GetCapsuleComponent()->SetCapsuleRadius(42.0f);

	GetMovementComponent()->NavAgentProps.AgentRadius = 38;
	GetMovementComponent()->NavAgentProps.AgentHeight = 192;

	MeleeCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeCollision"));
	MeleeCollisionComp->SetRelativeLocation(FVector(45, 0, 35));
	MeleeCollisionComp->SetCapsuleRadius(35, false);
	MeleeCollisionComp->SetCapsuleHalfHeight(60);
	MeleeCollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeCollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MeleeCollisionComp->SetupAttachment(GetCapsuleComponent());

	Health = 100;
	MeleeDamage = 24.0f;
	MeleeCooldown = 1.0f;

	SenseTimeout = 2.5f;

	CharacterMovementComp = GetCharacterMovement();

	DefaultMaxSpeed = 225.0f;
	SprintMaxSpeed = 450.0f;

	MonsterType = EMonsterBehaviorType::Patrolling;
}


void AMonsterCharacter::SetMonsterType(EMonsterBehaviorType NewType)
{
	MonsterType = NewType;

	AMonsterAIController* AIController = Cast<AMonsterAIController>(GetController());
	if (AIController)
	{
		AIController->SetBBMonsterType(NewType);
	}
}

void AMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSenseComp)
	{
		PawnSenseComp->OnSeePawn.AddDynamic(this, &AMonsterCharacter::OnSeePlayer);
		PawnSenseComp->OnHearNoise.AddDynamic(this, &AMonsterCharacter::OnHearNoise);
	}

	if (MeleeCollisionComp)
	{
		MeleeCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMonsterCharacter::OnMeleeCompBeginOverlap);
	}
}

void AMonsterCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bSensedTarget && (GetWorld()->TimeSeconds - LastSeenTime) > SenseTimeout && (GetWorld()->TimeSeconds - LastHeardTime) > SenseTimeout)
	{
		AMonsterAIController* AIController = Cast<AMonsterAIController>(GetController());
		if (AIController)
		{
			bSensedTarget = false;
			// Reset target
			AIController->SetTargetEnemy(nullptr);
			// Reset speed to walk
			CharacterMovementComp->MaxWalkSpeed = DefaultMaxSpeed;
		}
	}
}

void AMonsterCharacter::OnSeePlayer(APawn* Pawn)
{
	if (!IsAlive())
	{
		return;
	}

	LastSeenTime = GetWorld()->GetTimeSeconds();
	bSensedTarget = true;

	AMonsterAIController* AIController = Cast<AMonsterAIController>(GetController());
	AUEShooterCharacter* SensedPawn = Cast<AUEShooterCharacter>(Pawn);
	if (AIController && SensedPawn->IsAlive())
	{
		AIController->SetTargetEnemy(SensedPawn);
		CharacterMovementComp->MaxWalkSpeed = SprintMaxSpeed;
	}
}

void AMonsterCharacter::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	if (!IsAlive())
	{
		return;
	}

	LastHeardTime = GetWorld()->GetTimeSeconds();
	bSensedTarget = true;

 	AMonsterAIController* AIController = Cast<AMonsterAIController>(GetController());
 	if (AIController)
 	{
 		AIController->SetMoveToTarget(PawnInstigator);
	}
}

void AMonsterCharacter::OnMeleeCompBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Stop running timer
	TimerHandle_MeleeAttack.Invalidate();

	PerformMeleeStrike(OtherActor);

	// Set retrigger timer to recheck overlapped pawns at melee attack rate interval
	GetWorldTimerManager().SetTimer(TimerHandle_MeleeAttack, this, &AMonsterCharacter::OnRetriggerMeleeStrike, MeleeCooldown, true);

}

void AMonsterCharacter::OnRetriggerMeleeStrike()
{
	TArray<AActor*> Overlaps;
	MeleeCollisionComp->GetOverlappingActors(Overlaps, ABaseCharacter::StaticClass());
	for (int32 i = 0; i < Overlaps.Num(); i++)
	{
		ABaseCharacter* OverlappingPawn = Cast<ABaseCharacter>(Overlaps[i]);
		break;
	}

	if (Overlaps.Num() == 0)
	{
		TimerHandle_MeleeAttack.Invalidate();
	}
}

void AMonsterCharacter::PerformMeleeStrike(AActor* HitActor)
{
	if (LastMeleeAttackTime > GetWorld()->GetTimeSeconds() - MeleeCooldown)
	{
		// Set timer to start attacking
		if (!TimerHandle_MeleeAttack.IsValid())
		{
			GetWorldTimerManager().SetTimer(TimerHandle_MeleeAttack, this, &AMonsterCharacter::OnRetriggerMeleeStrike, MeleeCooldown, true);
		}

		return;
	}

	if (HitActor && HitActor != this && IsAlive())
	{
		AUEShooterCharacter* OtherPawn = Cast<AUEShooterCharacter>(HitActor);
		if (OtherPawn)
		{
			LastMeleeAttackTime = GetWorld()->GetTimeSeconds();

			FPointDamageEvent DmgEvent;
			DmgEvent.Damage = MeleeDamage;

			HitActor->TakeDamage(DmgEvent.Damage, DmgEvent, GetController(), this);
			
			if (MeleeAnimMontage != NULL)
			{
				PlayAnimMontage(MeleeAnimMontage);
			}
		}
	}
}

void AMonsterCharacter::PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser, bool bKilled)
{
	//Super::PlayHit(DamageTaken, DamageEvent, PawnInstigator, DamageCauser, bKilled);

}

float AMonsterCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (this->IsAlive())
	{
		// Play hit animation
		if (DamageTakenAnimMontage != NULL)
		{
			PlayAnimMontage(DamageTakenAnimMontage);
		}

		// Enemy should notice player when hit

		AMonsterAIController* AIController = Cast<AMonsterAIController>(GetController());
		AUEShooterProjectile* Projectile = Cast<AUEShooterProjectile>(DamageCauser);
		AUEShooterCharacter* PC = Cast<AUEShooterCharacter>(Projectile->GetPawnOwner());
		AIController->SetTargetEnemy(PC);
	}

	return ActualDamage;
}


UAudioComponent* AMonsterCharacter::PlayCharacterSound(USoundCue* CueToPlay)
{
	return nullptr;
}
