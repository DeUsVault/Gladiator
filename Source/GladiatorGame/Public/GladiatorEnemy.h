// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GladiatorCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GladiatorEnemy.generated.h"

UCLASS()
class GLADIATORGAME_API AGladiatorEnemy : public AGladiatorCharacter
{
	GENERATED_BODY()

	UPROPERTY() bool HasBeenTriggered;

public:
	AGladiatorEnemy(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void BeginPlay() override;
	
public:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly) UBehaviorTree* AggressiveBehavior;
};
