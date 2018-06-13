// Copyright Sean Duffy 2018

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIWaypoint.h"
#include "Types.h"
#include "UEShooterCharacter.h"
#include "MonsterAIController.generated.h"

/**
 * AI Controller for basic monster enemies
 */
UCLASS()
class UESHOOTER_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

	AMonsterAIController(const class FObjectInitializer& ObjectInitializer);

	virtual void Possess(class APawn* InPawn) override;

	virtual void UnPossess() override;

	class UBehaviorTreeComponent* BehaviorComp;

	class UBlackboardComponent* BBComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetLocationKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetEnemyKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName PatrolLocationKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName CurrentWaypointKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName MonsterTypeKeyName;

public:
	
	AAIWaypoint* GetWaypoint();

	AUEShooterCharacter* GetTargetEnemy();

	void SetWaypoint(AAIWaypoint* NewWaypoint);

	void SetTargetEnemy(APawn* NewTarget);

	void SetBBMonsterType(EMonsterBehaviorType NewType);

	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }

	FORCEINLINE UBlackboardComponent* GetBBComp() const { return BBComp; }

	void SetMoveToTarget(APawn* Pawn);
	
};
