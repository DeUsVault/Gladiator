// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GladiatorMeleeWeapon.generated.h"

UCLASS()
class GLADIATORGAME_API AGladiatorMeleeWeapon : public AActor
{
	GENERATED_BODY()

public:	
	AGladiatorMeleeWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WeaponMesh;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Damage;
};
