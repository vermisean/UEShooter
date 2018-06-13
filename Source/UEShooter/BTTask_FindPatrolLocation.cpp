// Copyright Sean Duffy 2018

#include "BTTask_FindPatrolLocation.h"
#include "AIWaypoint.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"

EBTNodeResult::Type UBTTask_FindPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMonsterAIController* MyController = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	if (MyController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No PATROL AIController found"));
		return EBTNodeResult::Failed;
	}

	AAIWaypoint* MyWaypoint = MyController->GetWaypoint();
	if (MyWaypoint)
	{
		const float SearchRadius = 200.0f;
		const FVector SearchOrigin = MyWaypoint->GetActorLocation();
		const FVector Loc = UNavigationSystem::GetRandomReachablePointInRadius(MyController, SearchOrigin, SearchRadius);
		if (Loc != FVector::ZeroVector)
		{
			/* The selected key should be "PatrolLocation" in the BehaviorTree setup */
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), Loc);
			return EBTNodeResult::Succeeded;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("No PATROL waypoint found"));
	return EBTNodeResult::Failed;
}
