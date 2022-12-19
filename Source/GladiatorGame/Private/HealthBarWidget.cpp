// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"

#include "GladiatorCharacter.h"

void UHealthBarWidget::SetOwnerCharacter(AGladiatorCharacter* Owner)
{
	OwnerCharacter = Owner;
	if (OwnerCharacter)
		OwnerCharacter->OnLifeUpdated.AddUniqueDynamic(this, &UHealthBarWidget::OnLifeUpdated);
}

void UHealthBarWidget::SetBarColor(const FLinearColor Color)
{
	HealthBar->FillColorAndOpacity = Color;
}


void UHealthBarWidget::OnLifeUpdated()
{
	if (OwnerCharacter)
		HealthBar->SetPercent(static_cast<float>(OwnerCharacter->Life) / OwnerCharacter->MaxLife);
}
