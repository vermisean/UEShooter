// Copyright Sean Duffy 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class UESHOOTER_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

	// Tracks noise used by pawn sense
	UPawnNoiseEmitterComponent* NoiseEmitterComponent;

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	//Health
	UFUNCTION(BlueprintCallable, Category = "Condition")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Condition")
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Condition")
	bool IsAlive() const;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Condition")
	float Health;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual bool CanDie(float KillingDamage, FDamageEvent const& DamageEvent) const;

	virtual bool Die(float KillingDamage, FDamageEvent const& DamageEvent);

	virtual void OnDeath(float KillingDamage, FDamageEvent const& DamageEvent);

	void SetRagDollPhysics();

	bool bIsDying;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};