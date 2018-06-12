// Copyright Sean Duffy 2018

// #include "BTTask_FindWaypoint.h"
// #include "AIWaypoint.h"
// #include "MonsterAIController.h"
// #include "BehaviorTree/BehaviorTreeComponent.h"
// #include "BehaviorTree/BlackboardComponent.h"
// #include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
// #include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
// 
// 
// 
// EBTNodeResult::Type UBTTask_FindWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
// {
// 	AMonsterAIController* MyController = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
// 	if (MyController == nullptr)
// 	{
// 		return EBTNodeResult::Failed;
// 	}
// 
// 	AAIWaypoint* CurrentWaypoint = MyController->GetWaypoint();
// 	AActor* NewWaypoint = nullptr;
// 
// 	// Iterate all waypoints in level and find new random waypoint
// 	TArray<AActor*> AllWaypoints;
// 	UGameplayStatics::GetAllActorsOfClass(MyController, AAIWaypoint::StaticClass(), AllWaypoints);
// 
// 	if (AllWaypoints.Num() == 0)
// 	{
// 		return EBTNodeResult::Failed;
// 	}
// 
// 	// Finds new waypoint by index - can include current waypoint TODO add weights based on environment/distance
// 	NewWaypoint = AllWaypoints[FMath::RandRange(0, AllWaypoints.Num() - 1)];
// 
// 	// Assign new waypoint
// 	if (NewWaypoint)
// 	{
// 		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID(), NewWaypoint);
// 		return EBTNodeResult::Succeeded;
// 	}
// 
// 	return EBTNodeResult::Failed;
// 
// 
// }
