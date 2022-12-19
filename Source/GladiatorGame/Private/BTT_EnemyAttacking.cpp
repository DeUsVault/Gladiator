// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_EnemyAttacking.h"

#include "AIController.h"
#include "GladiatorCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTT_EnemyAttacking::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	// Update latest player position blackboard value while attacking.
	const AGladiatorCharacter* PlayerCharacter = Cast<AGladiatorCharacter>(UGameplayStatics::GetPlayerCharacter(OwnerComp.GetWorld(), 0));
	OwnerComp.GetBlackboardComponent()->SetValueAsVector("LatestPlayerPosition", PlayerCharacter->GetActorLocation());

	// Attack.
	AGladiatorCharacter* Enemy = Cast<AGladiatorCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy)
	{
		Enemy->StartAttacking();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
