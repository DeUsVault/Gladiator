// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GladiatorMeleeWeapon.h"
#include "Components/WidgetComponent.h"
#include "GladiatorCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLifeUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKilled);

UCLASS()
class GLADIATORGAME_API AGladiatorCharacter : public ACharacter
{
	GENERATED_BODY()

	// Private main components.

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

public:
	AGladiatorCharacter(const FObjectInitializer& ObjectInitializer);

	// Public main components.

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	TSubclassOf<AGladiatorMeleeWeapon> MeleeWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* CounterAttackCollider;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	// Movements related methods.
	
	UFUNCTION(BlueprintCallable) void Walk();
	UFUNCTION(BlueprintCallable) void Run();
	void MoveForward(const float Value);
	void MoveRight(const float Value);

	// Actions related methods.
	
	UFUNCTION(BlueprintCallable) void StartRolling();
	void UpdateRolling();
	UFUNCTION(BlueprintCallable) void EndRolling();
	
	UFUNCTION(BlueprintCallable) void StartAttacking();
	UFUNCTION(BlueprintCallable) void EndAttacking();
	
	void StartBlocking();
	void EndBlocking();

	UFUNCTION() void StartCounterAttacking();
	UFUNCTION(BlueprintCallable) void EndCounterAttacking();

	UFUNCTION() void StartStaggering();
	UFUNCTION(BlueprintCallable) void EndStaggering();

	// States related methods.
	
	UFUNCTION() void IsBeingInjured(const int32 Damage);
	UFUNCTION(BlueprintCallable) void StopBeingInjured();
	UFUNCTION(BlueprintCallable) void Death();

	// States variables.
	
	UPROPERTY(BlueprintReadOnly,  Category = States)  bool CanMove;
	UPROPERTY(BlueprintReadOnly,  Category = States)  bool CanRoll;
	UPROPERTY(BlueprintReadOnly,  Category = States)  bool CanAttack;
	UPROPERTY(BlueprintReadOnly,  Category = States)  bool IsRunning;
	UPROPERTY(BlueprintReadOnly,  Category = States)  bool IsRolling;
	UPROPERTY(BlueprintReadOnly,  Category = States)  bool IsAttacking;
	UPROPERTY(BlueprintReadOnly,  Category = States)  bool IsBlocking;
	UPROPERTY(BlueprintReadOnly,  Category = States)  bool IsCounterAttacking;
	UPROPERTY(BlueprintReadOnly,  Category = States)  bool IsLocking;
	UPROPERTY(BlueprintReadOnly,  Category = States)  bool IsInjured;
	UPROPERTY(BlueprintReadWrite, Category = States)  bool IsStaggering;

	// Player stats variables.
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats) int32 Life;
	UPROPERTY() int32 MaxLife;

	UPROPERTY(BlueprintReadOnly, Category = Stats) float WalkSpeed;
	UPROPERTY(BlueprintReadOnly, Category = Stats) float RunSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	float InjuredRate = 1.f;

	// HUD related.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI) float EnemyHealthBarRenderDistance = 800.f;
	UPROPERTY(EditAnywhere) UWidgetComponent* HealthBarInstance;
	
	// Delegates variables.
	
	UPROPERTY(BlueprintAssignable) FOnLifeUpdated OnLifeUpdated;
	UPROPERTY(BlueprintAssignable) FOnLifeUpdated OnKilled;

	// Other variables.
	UPROPERTY() FRotator PawnYawRotator;
	UPROPERTY() FTimerHandle TimerHandler;
	UPROPERTY() TArray<UMaterialInstanceDynamic*> Materials;
	UPROPERTY(BlueprintReadOnly) AActor* MeleeWeaponInstance;
};
