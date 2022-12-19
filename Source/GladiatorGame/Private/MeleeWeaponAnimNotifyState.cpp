// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponAnimNotifyState.h"

#include "GladiatorCharacter.h"
#include "Animation/SkeletalMeshActor.h"
#include "Kismet/GameplayStatics.h"

void UMeleeWeaponAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	AActor* Owner = MeshComp->GetOwner();

	if (Owner)
	{
		TArray<AActor*> Enemies;
		TArray<FHitResult> HitResults;
		
		// Anti team kill check.
		if (Owner->ActorHasTag("Enemy"))
			UGameplayStatics::GetAllActorsWithTag(MeshComp, "Enemy", Enemies);
		else
			Enemies.Add(Owner);

		// Main collision check.
		const bool MeleeWeaponColliding = UKismetSystemLibrary::SphereTraceMultiForObjects
		(
			Owner->GetWorld(),
			MeshComp->GetSocketLocation("SocketTrail_Start"),
			MeshComp->GetSocketLocation("SocketTrail_End"),
			2.f,
			{
				UEngineTypes::ConvertToObjectType(ECC_Pawn),
				UEngineTypes::ConvertToObjectType(ECC_PhysicsBody)
			},
			false,
			Enemies,
			EDrawDebugTrace::None,
			HitResults,
			true
		);

		if (MeleeWeaponColliding)
		{
			for (FHitResult Hit : HitResults)
			{
				AGladiatorCharacter* Victim = Cast<AGladiatorCharacter>(Hit.Actor);
				const USkeletalMeshComponent* VictimSM = Cast<USkeletalMeshComponent>(Hit.Component);

				// Victim countering attack.
				if (Victim && Victim->Life > 0)
				{
					AGladiatorCharacter* Attacker = Cast<AGladiatorCharacter>(Owner);
					if (Attacker && Attacker->Life > 0)
					{
						const AGladiatorMeleeWeapon* AttackerWeapon = Cast<AGladiatorMeleeWeapon>(Attacker->MeleeWeaponInstance);
						if (AttackerWeapon)
						{
							if (Victim->IsBlocking && Victim->CounterAttackCollider->IsOverlappingActor(AttackerWeapon))
		                    {
								if (!Victim->IsCounterAttacking) Victim->StartCounterAttacking();
								if (!Victim->IsStaggering) Attacker->StartStaggering();
		                    }
						}
						
					}
				}

				// Victim injured.
	            if (VictimSM && !Victim->IsCounterAttacking && Victim->Life > 0)
	            {
            		const AGladiatorCharacter* Attacker = Cast<AGladiatorCharacter>(Owner);
					if (Attacker && Attacker->Life > 0)
					{
						const AGladiatorMeleeWeapon* AttackerWeapon = Cast<AGladiatorMeleeWeapon>(Attacker->MeleeWeaponInstance);
						if (AttackerWeapon && !Victim->IsInjured) Victim->IsBeingInjured(AttackerWeapon->Damage);
					}
	            }
            }
		}
	}
}
