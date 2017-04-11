// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGProject.h"
#include "BTTask_RandomMove.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "TrollAIController.h"



EBTNodeResult::Type UBTTask_RandomMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(GetWorld());

	ATrollAIController* MyController = Cast<ATrollAIController>(OwnerComp.GetAIOwner());

	if (NavSys && MyController && MyController->GetPawn())
	{
		const float SearchRadius = 200;
		FNavLocation RandomPt;

		const bool bFound = NavSys->GetRandomPointInNavigableRadius(MyController->GetPawn()->GetActorLocation(), SearchRadius, RandomPt);

		if (bFound)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(GetSelectedBlackboardKey(), RandomPt.Location);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}