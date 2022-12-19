// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GladiatorCharacter.h"
#include "GladiatorEnemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Actor.h"
#include "AIDirector.generated.h"

UCLASS()
class GLADIATORGAME_API AAIDirector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIDirector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void ToggleEnemyAggressiveState();

	UPROPERTY(BlueprintReadOnly)
	TArray<AGladiatorEnemy*> Enemies;

	UPROPERTY(EditAnywhere)
	float EnemyTriggeredTimer = 10.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AGladiatorEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBehaviorTree* BTPassive;

	int EnemyRemainingToTrigger;
	FTimerHandle TimerHandle;
};
