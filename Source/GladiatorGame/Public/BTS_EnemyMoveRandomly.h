// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_EnemyMoveRandomly.generated.h"

UCLASS()
class GLADIATORGAME_API UBTS_EnemyMoveRandomly : public UBTService
{
	GENERATED_BODY()

	UPROPERTY() UWorld* CurrentWorld;
	UPROPERTY() AAIController* CurrentAIController;
	UPROPERTY() UBlackboardComponent* CurrentBlackboard;
	UPROPERTY() APawn* CurrentPawn;

public:
	UBTS_EnemyMoveRandomly(const FObjectInitializer& ObjectInitializer);
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable)
	bool CheckPlayerRange(const float Min, const float Max) const;
	
	UPROPERTY(EditAnywhere, Category = "Random Movements") float RMovementsRadius = 3000.f;

};
