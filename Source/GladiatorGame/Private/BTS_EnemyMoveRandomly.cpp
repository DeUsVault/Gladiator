// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_EnemyMoveRandomly.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UBTS_EnemyMoveRandomly::UBTS_EnemyMoveRandomly(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyTick = true;
}


bool UBTS_EnemyMoveRandomly::CheckPlayerRange(const float Min, const float Max) const
{
	const FVector PlayerLocation = UGameplayStatics::GetPlayerCharacter(CurrentWorld, 0)->GetActorLocation();
	const FVector EnemyLocation  = CurrentPawn->GetActorLocation();
	return UKismetMathLibrary::InRange_FloatFloat(FVector::Distance(PlayerLocation, EnemyLocation), Min, Max, false, true);
}

void UBTS_EnemyMoveRandomly::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	CurrentWorld = OwnerComp.GetWorld();
	CurrentAIController = OwnerComp.GetAIOwner();
	CurrentPawn = CurrentAIController->GetPawn();
	CurrentBlackboard = CurrentAIController->GetBlackboardComponent();

	if (CurrentWorld && CurrentAIController && CurrentPawn && CurrentBlackboard)
	{
		if (!CheckPlayerRange(0.f, 600.f) && !CurrentBlackboard->GetValueAsBool("IsDodging"))
		{
			const FVector PlayerLocation = UGameplayStatics::GetPlayerCharacter(CurrentWorld, 0)->GetActorLocation();
			const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	
			FNavLocation Result;
			NavSystem->GetRandomPointInNavigableRadius(PlayerLocation, RMovementsRadius, Result);
			if (FVector::Distance(PlayerLocation, Result.Location) >= 600.f)
				CurrentAIController->MoveToLocation(Result.Location);
		}
	}
}