// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_CheckPlayerInRange.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UBTD_CheckPlayerInRange : public UBTDecorator
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) float Range = 150.f;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
