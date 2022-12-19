// Fill out your copyright notice in the Description page of Project Settings.


#include "GladiatorEnemy.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AGladiatorEnemy::AGladiatorEnemy(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HasBeenTriggered = false;
}

void AGladiatorEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AGladiatorEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!(IsAttacking || IsCounterAttacking))
	{
		const float EnemyYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
			UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()).Yaw;
		SetActorRotation({ 0.f, EnemyYaw, 0.f });
	}

	if (!HasBeenTriggered)
	{
		AAIController* EnemyController = Cast<AAIController>(GetController());
		if (EnemyController)
		{
			const UBlackboardComponent* EnemyBlackboard = EnemyController->GetBlackboardComponent();
			if (EnemyBlackboard && EnemyBlackboard->GetValueAsBool("IsAggressive"))
			{
				EnemyController->RunBehaviorTree(AggressiveBehavior);
				HasBeenTriggered = true;
			}
		}
	}
}
