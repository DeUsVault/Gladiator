// Fill out your copyright notice in the Description page of Project Settings.

#include "GladiatorCharacter.h"

#include "GladiatorGame/Includes/MyDebug.h"

#include "GameManager.h"
#include "HealthBarWidget.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AGladiatorCharacter::AGladiatorCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Configure rotations.
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 650.f, 0.f);
	
	// Initialize components.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	if (SpringArm)
	{
		SpringArm->SetupAttachment(RootComponent);
		SpringArm->TargetArmLength = 300.0f;
		SpringArm->bUsePawnControlRotation = true;
	}

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if (Camera)
	{
		Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
		Camera->bUsePawnControlRotation = false;
	}

	CounterAttackCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("CounterAttackCollider"));
	if (CounterAttackCollider)
		CounterAttackCollider->SetupAttachment(RootComponent);

	// Set default values.
	Life = 5;
	WalkSpeed = 300.f;
	RunSpeed  = 600.f;

	// Setup character HUD elements.
	HealthBarInstance = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("HealthBar"));
	HealthBarInstance->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AGladiatorCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set default states.
	CanMove     = true;
	CanRoll     = true;
	CanAttack   = true;
	IsRolling   = false;
	IsAttacking = false;
	IsBlocking  = false;
	IsLocking   = false;
	IsInjured   = false;
	IsCounterAttacking = false;
	MaxLife = Life;

	// Set default walk speed.
	Walk();
	
	// Attach weapon to gladiator mesh.
	if (MeleeWeapon)
	{
		const FTransform MeleeWeaponTransform = GetMesh()->GetSocketTransform("Socket_Weapon");
		FActorSpawnParameters MeleeWeaponSpawnParams;
		MeleeWeaponSpawnParams.Owner = this;
		
		MeleeWeaponInstance = GetWorld()->SpawnActor(MeleeWeapon, &MeleeWeaponTransform, MeleeWeaponSpawnParams);
		if (MeleeWeaponInstance)
			MeleeWeaponInstance->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Socket_Weapon");
	}

	// Set character master material.
	for (int i = 0; i < GetMesh()->GetMaterials().Num(); i++)
		Materials.Add(GetMesh()->CreateAndSetMaterialInstanceDynamic(i));

	// Setup health bar widget.
	UHealthBarWidget* HealthBarWidget = Cast<UHealthBarWidget>(HealthBarInstance->GetUserWidgetObject());
	if (HealthBarWidget)
	{
		HealthBarWidget->SetOwnerCharacter(this);
		if (IsPlayerControlled())
		{
			HealthBarInstance->SetVisibility(false);
			HealthBarWidget->SetBarColor(FLinearColor::Green);
		}
		else
		{
			HealthBarInstance->SetVisibility(true);
			HealthBarWidget->SetBarColor(FLinearColor::Red);
		}
	}

	// Game Manager related.
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "GameManager", OutActors);
	if (OutActors.Num() > 0)
		Cast<AGameManager>(OutActors[0])->BindOnKilledDelegate(this);
}

void AGladiatorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update current pawn rotation (frozen during rolling or attacking).
	if (CanMove)
		PawnYawRotator = { 0, Controller->GetControlRotation().Yaw, 0 };

	// Update pawn health bar visibility.
	if (ActorHasTag("Enemy") && HealthBarInstance)
	{
		const FVector PlayerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
		HealthBarInstance->SetVisibility(FVector::Distance(GetActorLocation(), PlayerLocation) <= EnemyHealthBarRenderDistance);
	}
	
	UpdateRolling();
}

void AGladiatorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Action binding.
	InputComponent->BindAction("Run",   IE_Pressed,  this, &AGladiatorCharacter::Run);
	InputComponent->BindAction("Run",   IE_Released, this, &AGladiatorCharacter::Walk);
	InputComponent->BindAction("Roll",  IE_Pressed,  this, &AGladiatorCharacter::StartRolling);
	InputComponent->BindAction("Attack",IE_Pressed,  this, &AGladiatorCharacter::StartAttacking);
	InputComponent->BindAction("Block", IE_Pressed,  this, &AGladiatorCharacter::StartBlocking);
	InputComponent->BindAction("Block", IE_Released, this, &AGladiatorCharacter::EndBlocking);

	// Axis binding.
	InputComponent->BindAxis("MoveForward", this, &AGladiatorCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight",   this, &AGladiatorCharacter::MoveRight);
	InputComponent->BindAxis("LookUp",      this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookRight",   this, &APawn::AddControllerYawInput);
}

void AGladiatorCharacter::Walk()
{
	if (!IsRolling) GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	IsRunning = false;
}

void AGladiatorCharacter::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	IsRunning = true;
}

void AGladiatorCharacter::MoveForward(const float Value)
{
	if (Controller != nullptr && Value != 0.0f && CanMove && !IsBlocking)
		AddMovementInput(FRotationMatrix(PawnYawRotator).GetUnitAxis(EAxis::X), Value);
}

void AGladiatorCharacter::MoveRight(const float Value)
{
	if (Controller != nullptr && Value != 0.0f && CanMove && !IsBlocking)
		AddMovementInput(FRotationMatrix(PawnYawRotator).GetUnitAxis(EAxis::Y), Value);
}

/*
 * Actions related methods.
*/

void AGladiatorCharacter::StartRolling()
{
	const FVector Velocity = GetCharacterMovement()->Velocity;
	if (CanRoll && !IsAttacking && !IsBlocking && !IsCounterAttacking && Velocity.SizeSquared() >= 1000.f)
	{
		IsRolling = true; CanRoll = CanMove = false;
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void AGladiatorCharacter::UpdateRolling()
{
	if (IsRolling && !IsAttacking && !IsBlocking && !IsCounterAttacking)
		AddMovementInput(GetCharacterMovement()->Velocity, 1.0f);
}

void AGladiatorCharacter::EndRolling()
{
	IsRolling = false; CanRoll = CanMove = CanAttack = true;
	if (!IsRunning) Walk();
}

void AGladiatorCharacter::StartAttacking()
{
	if (CanAttack && !IsRolling && !IsBlocking && !IsCounterAttacking)
	{
		IsAttacking = true; CanRoll = CanMove = CanAttack = false;
		Walk();
	}
}

void AGladiatorCharacter::EndAttacking()
{
	IsAttacking = false; CanRoll = CanMove = CanAttack = true;
}

void AGladiatorCharacter::StartBlocking()
{
	if (!IsAttacking || !IsRolling)
	{
		IsBlocking = true;
		CanRoll = CanMove = CanAttack = false;
	}
}

void AGladiatorCharacter::EndBlocking()
{
	IsBlocking = false;
	if (!IsCounterAttacking)
		CanRoll = CanMove = CanAttack = true;
}

void AGladiatorCharacter::StartCounterAttacking()
{
	IsCounterAttacking = true;
}

void AGladiatorCharacter::EndCounterAttacking()
{
	IsCounterAttacking = false;
	if (!IsBlocking)
		CanRoll = CanMove = CanAttack = true;
}

void AGladiatorCharacter::StartStaggering()
{
	IsStaggering = true;
	IsAttacking = false;
	CanRoll = CanMove = CanAttack = false;
}

void AGladiatorCharacter::EndStaggering()
{
	IsStaggering = false;
	CanRoll = CanMove = CanAttack = true;
}

void AGladiatorCharacter::IsBeingInjured(const int32 Damage)
{
	if (!IsInjured)
	{
		if (Life - Damage <= 0)
		{
			OnKilled.Broadcast();
			GetMesh()->GetAnimInstance()->StopSlotAnimation();
			Death();
		}
		else
		{
			Life -= Damage;
			IsInjured = CanMove = true;
			CanAttack = CanRoll = IsAttacking = IsStaggering = IsRolling = IsBlocking = false;

			// Trigger material and UI injured state.
			for (int i = 0; i < GetMesh()->GetMaterials().Num(); i++)
				Materials[i]->SetScalarParameterValue("Injured", 1.f);
			HealthBarInstance->SetVisibility(true);

			GetWorldTimerManager().SetTimer(TimerHandler, this, &AGladiatorCharacter::StopBeingInjured, InjuredRate, false);
			OnLifeUpdated.Broadcast();
		}
	}
}

void AGladiatorCharacter::StopBeingInjured()
{
	IsInjured = false;
	CanRoll = CanAttack = true;

	// Reset material and ui state.
	for (int i = 0; i < GetMesh()->GetMaterials().Num(); i++)
		Materials[i]->SetScalarParameterValue("Injured", 0.f);
	HealthBarInstance->SetVisibility(false);
}


void AGladiatorCharacter::Death()
{
	Life = 0;

	// Enable ragdoll.
	GetMesh()->SetSimulatePhysics(true);
	GetCharacterMovement()->DisableMovement();
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (Capsule)
	{
		Capsule->SetActive(false);
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	// Destroy components.
	MeleeWeaponInstance->Destroy();
	HealthBarInstance->DestroyComponent();

	// Unbind AI Controller to its attached behavior tree (if there's one).
	const AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
		AIController->BrainComponent->StopLogic("Dead");
}

