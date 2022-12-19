// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_EnemyPassiveMovements.generated.h"

UCLASS()
class GLADIATORGAME_API UBTT_EnemyPassiveMovements : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	UPROPERTY() const UWorld* CurrentWorld;
	UPROPERTY() AAIController* CurrentAIController;
	UPROPERTY() const APawn*  CurrentPawn;
	UPROPERTY() UBlackboardComponent* CurrentBlackboard;
	UPROPERTY() ACharacter* Player;
	UPROPERTY() FTimerHandle TimerHandle;

public:
	UFUNCTION(BlueprintCallable)
	void DodgePlayer();
	
	UFUNCTION(BlueprintCallable)
	bool CheckPlayerRange(const float Min, const float Max) const;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
