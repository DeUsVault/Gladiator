// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_UpdateBlackboard.h"

#include "AIController.h"
#include "GladiatorCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void UBTS_UpdateBlackboard::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	const AGladiatorCharacter*  ControlledEnemy = Cast<AGladiatorCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent*		EnemyBlackboard = OwnerComp.GetBlackboardComponent();
	
	EnemyBlackboard->SetValueAsBool("IsAlive", ControlledEnemy->Life > 0);
	EnemyBlackboard->SetValueAsVector("LatestPlayerRotation", UGameplayStatics::GetPlayerCharacter(OwnerComp.GetWorld(), 0)->GetActorLocation());
}
