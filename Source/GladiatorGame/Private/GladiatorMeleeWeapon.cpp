// Fill out your copyright notice in the Description page of Project Settings.

#include "GladiatorMeleeWeapon.h"

AGladiatorMeleeWeapon::AGladiatorMeleeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	// Initialize main components.
	WeaponMesh      = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	RootComponent  = WeaponMesh;
}

void AGladiatorMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AGladiatorMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}