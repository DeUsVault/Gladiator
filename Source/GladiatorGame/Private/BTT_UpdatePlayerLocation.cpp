// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_UpdatePlayerLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTT_UpdatePlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	OwnerComp.GetBlackboardComponent()->SetValueAsVector("LatestPlayerPosition", 
	UGameplayStatics::GetPlayerCharacter(OwnerComp.GetWorld(), 0)->GetActorLocation());
	return EBTNodeResult::Succeeded;
}
