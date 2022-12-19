// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CheckBothAlive.h"

#include "GladiatorCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

class AGladiatorCharacter;

bool UBTD_CheckBothAlive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	const AGladiatorCharacter* Player = Cast<AGladiatorCharacter>(UGameplayStatics::GetPlayerCharacter(OwnerComp.GetWorld(), 0));
	const UBlackboardComponent* EnemyBlackboard = OwnerComp.GetBlackboardComponent();
	return EnemyBlackboard->GetValueAsBool("IsAlive") && Player->Life > 0;
}
