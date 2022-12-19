// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyPassiveMovements.h"

#include "AIController.h"
#include "GladiatorCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UBTT_EnemyPassiveMovements::DodgePlayer()
{
	CurrentBlackboard->SetValueAsBool("IsDodging", true);
	const FVector PlayerVelocity = Player->GetVelocity().GetSafeNormal() * 150.f;
	CurrentAIController->MoveToLocation(CurrentPawn->GetActorLocation() + PlayerVelocity);
}

bool UBTT_EnemyPassiveMovements::CheckPlayerRange(const float Min, const float Max) const
{
	const FVector PlayerLocation = Player->GetActorLocation();
	const FVector EnemyLocation  = CurrentPawn->GetActorLocation();
	return UKismetMathLibrary::InRange_FloatFloat(FVector::Distance(PlayerLocation, EnemyLocation), Min, Max, false, true);
}

EBTNodeResult::Type UBTT_EnemyPassiveMovements::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	CurrentWorld = OwnerComp.GetWorld();
	CurrentAIController = OwnerComp.GetAIOwner();
	CurrentBlackboard = CurrentAIController->GetBlackboardComponent();
	CurrentPawn = CurrentAIController->GetPawn();
	Player = UGameplayStatics::GetPlayerCharacter(OwnerComp.GetWorld(), 0);

	if (CurrentWorld && CurrentAIController && CurrentBlackboard && CurrentPawn)
	{
		if (CheckPlayerRange(0.f, 250.f))
			CurrentBlackboard->SetValueAsBool("IsAggressive", true);
		else if (CheckPlayerRange(250.f, 600.f) && Player->GetVelocity().SizeSquared() >= 100.f)
			DodgePlayer();
		else
			CurrentBlackboard->SetValueAsBool("IsDodging", false);
		
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
