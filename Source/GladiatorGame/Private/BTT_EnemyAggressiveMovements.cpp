// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_EnemyAggressiveMovements.h"

#include "AIController.h"
#include "GladiatorCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTT_EnemyAggressiveMovements::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	
	const AGladiatorCharacter* Player = Cast<AGladiatorCharacter>(UGameplayStatics::GetPlayerCharacter(OwnerComp.GetWorld(), 0));
	const AGladiatorCharacter* Enemy  = Cast<AGladiatorCharacter>(AIController->GetPawn());

	const FVector PlayerLocation       = Player->GetActorLocation();
	const FVector EnemyLocation        = Enemy->GetActorLocation();
	const FVector PlayerSharedVelocity = Player->GetVelocity().GetSafeNormal() * 150.f;
	
	AIController->GetBlackboardComponent()->SetValueAsVector("LatestPlayerPosition", PlayerLocation);

	const bool IsDodging =
		Enemy && !Enemy->IsAttacking &&
		!(Player->IsAttacking || Player->IsRolling) &&
		FVector::Distance(PlayerLocation + PlayerSharedVelocity, EnemyLocation) <= 400.f &&
		Player->GetVelocity().SizeSquared() >= 100.f;
	
	if (IsDodging)
		AIController->MoveToLocation(EnemyLocation + PlayerSharedVelocity);
	else
		AIController->MoveToLocation(PlayerLocation);
	
	return EBTNodeResult::Succeeded;
}