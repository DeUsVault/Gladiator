// Fill out your copyright notice in the Description page of Project Settings.


#include "AIDirector.h"

#include "AIController.h"
#include "GladiatorEnemy.h"
#include "TimerManager.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAIDirector::AAIDirector()
{
 	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAIDirector::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Enemy", OutActors);

	// Add and initialize enemies' AI.
	for (int i = 0; i < OutActors.Num(); i++)
	{
		Enemies.Add(Cast<AGladiatorEnemy>(OutActors[i]));
		
		AAIController* EnemyAIController = Cast<AAIController>(Enemies[i]->GetController());
		if (EnemyAIController)
		{
			EnemyAIController->RunBehaviorTree(BTPassive);
			UBlackboardComponent* EnemyBlackboardComponent = EnemyAIController->GetBlackboardComponent();
			if (EnemyBlackboardComponent)
			{
				EnemyBlackboardComponent->SetValueAsBool("RightDirection", UKismetMathLibrary::RandomBool());
				EnemyBlackboardComponent->SetValueAsVector("LatestPlayerPosition", UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
			}
		}
	}

	// Setup enemy triggering timer context.
	EnemyRemainingToTrigger = Enemies.Num();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AAIDirector::ToggleEnemyAggressiveState, EnemyTriggeredTimer, true);
}

void AAIDirector::ToggleEnemyAggressiveState()
{
	if (EnemyRemainingToTrigger > 0)
	{
		for (const AGladiatorEnemy* Enemy : Enemies)
		{
			AAIController* EnemyAIController = Cast<AAIController>(Enemy->GetController());
			if (EnemyAIController)
			{
				const UBlackboardComponent* EnemyBlackboardComponent = EnemyAIController->GetBlackboardComponent();
				if (EnemyBlackboardComponent && !EnemyBlackboardComponent->GetValueAsBool("IsAggressive"))
				{
					EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsAggressive", true);
					EnemyRemainingToTrigger--;
					break;
				}
			}
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
}
