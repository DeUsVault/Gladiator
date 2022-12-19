// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState_Trail.h"
#include "MeleeWeaponAnimNotifyState.generated.h"

UCLASS()
class GLADIATORGAME_API UMeleeWeaponAnimNotifyState : public UAnimNotifyState_Trail
{
	GENERATED_BODY()

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
};
