// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameOverScreen.h"
#include "GladiatorEnemy.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

UCLASS()
class GLADIATORGAME_API AGameManager : public AActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY() int RemainingEnemies;
	UPROPERTY() FTimerHandle TimerHandle;

public:
	UFUNCTION() void BindOnKilledDelegate(AGladiatorCharacter* OwnerCharacter);
	UFUNCTION() void OnEnemyKilled();
	UFUNCTION() void TriggerWinScreen();
	UFUNCTION() void TriggerLooseScreen();
	UFUNCTION() void RestartGame();

	UPROPERTY(EditAnywhere) TSubclassOf<UUserWidget> GameOverScreenClass;
	UPROPERTY() UGameOverScreen* GameOverScreenInstance;
};
