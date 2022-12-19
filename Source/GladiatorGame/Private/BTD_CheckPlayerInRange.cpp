// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckPlayerInRange.h"

#include "GladiatorGame/Includes/MyDebug.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTD_CheckPlayerInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	const FVector EnemyLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	const UBlackboardComponent* EnemyBlackboard = OwnerComp.GetBlackboardComponent();
	const float PlayerDistance = FVector::Distance(EnemyLocation, EnemyBlackboard->GetValueAsVector("LatestPlayerPosition"));
	return  PlayerDistance <= Range;
}
