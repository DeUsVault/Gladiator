// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

#include "GameOverScreen.h"
#include "GladiatorEnemy.h"
#include "Kismet/GameplayStatics.h"

void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Enemy", OutActors);
	RemainingEnemies = OutActors.Num();

	if (IsValid(GameOverScreenClass))
	{
		GameOverScreenInstance = Cast<UGameOverScreen>(CreateWidget(GetWorld(), GameOverScreenClass));
		if (GameOverScreenInstance)
		{
			GameOverScreenInstance->AddToViewport();
			GameOverScreenInstance->SetVisibility(ESlateVisibility::Hidden);
		}
		
	}
}

void AGameManager::BindOnKilledDelegate(AGladiatorCharacter* OwnerCharacter)
{
	if (OwnerCharacter)
	{
		if (OwnerCharacter->IsPlayerControlled())
			OwnerCharacter->OnKilled.AddUniqueDynamic(this, &AGameManager::TriggerLooseScreen);
		else
			OwnerCharacter->OnKilled.AddUniqueDynamic(this, &AGameManager::OnEnemyKilled);
	}
}

void AGameManager::OnEnemyKilled()
{
	if (RemainingEnemies - 1 > 0)
		RemainingEnemies--;
	else
		TriggerWinScreen();
}

void AGameManager::TriggerWinScreen()
{
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AGameManager::RestartGame, 5.f, false);
	if (GameOverScreenInstance)
	{
		GameOverScreenInstance->GameOverText->SetText(FText::FromString("YOU WIN !"));
		GameOverScreenInstance->SetVisibility(ESlateVisibility::Visible);
	}
}

void AGameManager::TriggerLooseScreen()
{
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AGameManager::RestartGame, 5.f, false);
	if (GameOverScreenInstance)
	{
		GameOverScreenInstance->GameOverText->SetText(FText::FromString("YOU DIED"));
		GameOverScreenInstance->SetVisibility(ESlateVisibility::Visible);
	}
}

void AGameManager::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

