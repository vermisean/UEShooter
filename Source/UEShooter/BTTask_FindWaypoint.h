// Copyright Sean Duffy 2018
// 
 #pragma once
 
 #include "CoreMinimal.h"
 #include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
 #include "BTTask_FindWaypoint.generated.h"
 
 /**
  * Blackboard Task - Find next waypoint
  */
 UCLASS()
 class UESHOOTER_API UBTTask_FindWaypoint : public UBTTask_BlackboardBase
 {
 	GENERATED_BODY()
 
 	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
 	
 	
 };
