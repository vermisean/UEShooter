// Copyright Sean Duffy 2018

#include "MonsterAIController.h"
#include "MonsterCharacter.h"
#include "UEShooterCharacter.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"





AMonsterAIController::AMonsterAIController(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BBComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	TargetLocationKeyName = "TargetLocation";
	PatrolLocationKeyName = "PatrolLocation";
	CurrentWaypointKeyName = "CurrentWaypoint";
	MonsterTypeKeyName = "MonsterType";
	TargetEnemyKeyName = "TargetEnemy";
}

void AMonsterAIController::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);

	AMonsterCharacter* Monster = Cast<AMonsterCharacter>(InPawn);
	if (Monster)
	{
		if (Monster->BehaviorTree->BlackboardAsset)
		{
			BBComp->InitializeBlackboard(*Monster->BehaviorTree->BlackboardAsset);

			SetBBMonsterType(Monster->MonsterType);
		}

		BehaviorComp->StartTree(*Monster->BehaviorTree);
	}
}

void AMonsterAIController::UnPossess()
{
	Super::UnPossess();

	BehaviorComp->StopTree();
}

AAIWaypoint* AMonsterAIController::GetWaypoint()
{
	if (BBComp)
	{
		return Cast<AAIWaypoint>(BBComp->GetValueAsObject(CurrentWaypointKeyName));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("No AIController Waypoint found"));
	return nullptr;
}

AUEShooterCharacter* AMonsterAIController::GetTargetEnemy()
{
	if (BBComp)
	{
		return Cast<AUEShooterCharacter>(BBComp->GetValueAsObject(TargetEnemyKeyName));
	}

	return nullptr;
}

void AMonsterAIController::SetWaypoint(AAIWaypoint* NewWaypoint)
{
	if (BBComp)
	{
		BBComp->SetValueAsObject(CurrentWaypointKeyName, NewWaypoint);
	}
}

void AMonsterAIController::SetTargetEnemy(APawn* NewTarget)
{
	if (BBComp)
	{
		BBComp->SetValueAsObject(TargetEnemyKeyName, NewTarget);
	}
}

void AMonsterAIController::SetBBMonsterType(EMonsterBehaviorType NewType)
{
	if (BBComp)
	{
		BBComp->SetValueAsEnum(MonsterTypeKeyName, (uint8)NewType);
	}
}

void AMonsterAIController::SetMoveToTarget(APawn* Pawn)
{
	if (BBComp)
	{
		SetTargetEnemy(Pawn);

		if (Pawn)
		{
			BBComp->SetValueAsVector(TargetLocationKeyName, Pawn->GetActorLocation());
		}
	}
}
