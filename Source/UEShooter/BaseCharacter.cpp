// Copyright Sean Duffy 2018

#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 100.0f;

	NoiseEmitterComponent = CreateDefaultSubobject <UPawnNoiseEmitterComponent>(TEXT("Noise Emitter"));

}

float ABaseCharacter::GetMaxHealth() const
{
	return GetClass()->GetDefaultObject<ABaseCharacter>()->Health;
}

float ABaseCharacter::GetCurrentHealth() const
{
	return Health;
}

bool ABaseCharacter::IsAlive() const
{
	return Health > 0.0f;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

float ABaseCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (Health <= 0.0f)
	{
		return 0.0f;
	}

	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.0f)
	{
		Health -= ActualDamage;
		if (Health <= 0.0f)
		{
			bool bCanDie = true;

			if (bCanDie)
			{
				Die(ActualDamage, DamageEvent);
			}
		}
	}

	return ActualDamage;
}

bool ABaseCharacter::CanDie(float KillingDamage, FDamageEvent const& DamageEvent) const
{
	if ((bIsDying) || IsPendingKill())
	{
		return false;
	}

	return true;
}

bool ABaseCharacter::Die(float KillingDamage, FDamageEvent const& DamageEvent)
{
	if (!CanDie(KillingDamage, DamageEvent))
	{
		return false;
	}

	Health = FMath::Min(0.0f, Health);
	OnDeath(KillingDamage, DamageEvent);

	return true;
}

void ABaseCharacter::OnDeath(float KillingDamage, FDamageEvent const& DamageEvent)
{
	if (bIsDying)
	{
		return;
	}

	DetachFromControllerPendingDestroy();

	SetRagDollPhysics();
}

void ABaseCharacter::SetRagDollPhysics()
{
	bool bInRagdoll = false;
	USkeletalMeshComponent* Mesh = GetMesh();

	if (IsPendingKill())
	{
		bInRagdoll = false;
	}
	else if (!Mesh || !Mesh->GetPhysicsAsset())
	{
		bInRagdoll = false;
	}
	else
	{
		Mesh->SetAllBodiesSimulatePhysics(true);
		Mesh->SetSimulatePhysics(true);
		Mesh->WakeAllRigidBodies();
		Mesh->bBlendPhysics = true;

		bInRagdoll = true;
	}

	UCharacterMovementComponent* MoveComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (MoveComponent)
	{
		MoveComponent->StopMovementImmediately();
		MoveComponent->DisableMovement();
		MoveComponent->SetComponentTickEnabled(false);
	}

	if (bInRagdoll)
	{
		TurnOff();
		SetActorHiddenInGame(true);
		SetLifeSpan(1.0f);
	}
	else
	{
		SetLifeSpan(10.0f);
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

