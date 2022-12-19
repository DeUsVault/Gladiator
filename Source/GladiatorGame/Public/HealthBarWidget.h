// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GladiatorCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthBarWidget.generated.h"

UCLASS()
class GLADIATORGAME_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	AGladiatorCharacter* OwnerCharacter;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* HealthBar;

public:
	UFUNCTION() void SetOwnerCharacter(AGladiatorCharacter* Owner);
	UFUNCTION() void SetBarColor(const FLinearColor Color);

	UFUNCTION() void OnLifeUpdated();
};
