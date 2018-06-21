// Copyright Sean Duffy 2018

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Types.h"
#include "MonsterCharacter.generated.h"

/**
 * 
 */
UCLASS()
class UESHOOTER_API AMonsterCharacter : public ABaseCharacter
{
	GENERATED_BODY()

	float LastSeenTime;

	float LastHeardTime;

	float LastMeleeAttackTime;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UCharacterMovementComponent* CharacterMovementComp;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSenseComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SenseTimeout;

	bool bSensedTarget;
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

protected:

	//virtual void IsSprinting() const override;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DefaultMaxSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintMaxSpeed;

	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	class UCapsuleComponent* MeleeCollisionComp;

	UFUNCTION()
	void OnMeleeCompBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnRetriggerMeleeStrike();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void PerformMeleeStrike(AActor* HitActor);

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float MeleeDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	class UAnimMontage* MeleeAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	class UAnimMontage* DamageTakenAnimMontage;

	FTimerHandle TimerHandle_MeleeAttack;

	FTimerHandle TimerHandle_ResumeMovement;

	float MeleeCooldown;

	float MovementCooldown;

	void ResumeMovement();

	virtual void PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser, bool bKilled);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	class UAudioComponent* PlayCharacterSound(class USoundCue* CueToPlay);

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundPlayerNoticed;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundHunting;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundIdle;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundWandering;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundAttackMelee;

public:


	AMonsterCharacter(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	bool bIsAttacking;

	UPROPERTY(EditAnywhere, Category = "AI")
	EMonsterBehaviorType MonsterType;

	void SetMonsterType(EMonsterBehaviorType NewType);
	 
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

};
