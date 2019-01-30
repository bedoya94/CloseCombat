// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SwordMasters.h"
#include "SwordMastersCharacter.h"
#include "Enemy.h"

inline static bool ArrangeEnemiesByDirectionPredicate(const AEnemy &enemy1, const AEnemy &enemy2);

//////////////////////////////////////////////////////////////////////////
// ASwordMastersCharacter

ASwordMastersCharacter::ASwordMastersCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	combatComp = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("CombatComp"));
	ActorsDetector = CreateDefaultSubobject<USphereComponent>(TEXT("ActorSphere"));
	ActorsDetector->AttachTo(GetRootComponent());
	WeaponCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
}

void ASwordMastersCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	this->AddOwnedComponent(combatComp);
	ActorsDetector->SetSphereRadius(600.f);
}

void ASwordMastersCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ASwordMastersCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ActorsDetector->GetOverlappingActors(AllEnemies, AEnemy::StaticClass());
	CollectEnemies();
	Enemies.Sort(ArrangeEnemiesByDirectionPredicate);
	LookAtTarget();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASwordMastersCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("Attack", IE_Pressed, combatComp, &UHeroCombatComponent::AttackLogic);
	InputComponent->BindAction("Attack", IE_Released, combatComp, &UHeroCombatComponent::AttackEnd);

	InputComponent->BindAxis("MoveForward", this, &ASwordMastersCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ASwordMastersCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ASwordMastersCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ASwordMastersCharacter::LookUpAtRate);
}

void ASwordMastersCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASwordMastersCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASwordMastersCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASwordMastersCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ASwordMastersCharacter::CollectEnemies()
{
	if (AllEnemies.Num() > 0)
	{
		for (int i = 0; i < AllEnemies.Num(); i++)
		{
			AEnemy* const enemy = Cast<AEnemy>(AllEnemies[i]);

			if (enemy)
			{
				if (!Enemies.Contains(enemy))
				{
					Enemies.Add(enemy);
				}
			}
		}
	}

	if (Enemies.Num() > 0)
	{
		CurrentTarget = Enemies[0];
	}
}

inline static bool ArrangeEnemiesByDirectionPredicate(const AEnemy &enemy1, const AEnemy &enemy2)
{
	float dir1 = enemy1.direction;
	float dir2 = enemy2.direction;

	return dir1 > dir2;
}

void ASwordMastersCharacter::LookAtTarget()
{
	if (CurrentTarget)
	{
		FVector MeshLoc = GetActorLocation();
		FVector TargetLoc = CurrentTarget->GetActorLocation();

		FVector Direction = TargetLoc - MeshLoc;
		Direction.Normalize();

		FRotator MeshRot = Direction.Rotation();
		FRotator LookRot = Direction.Rotation();

		MeshRot.Pitch = 0.f;
		MeshRot.Yaw -= 90.f;
		GetMesh()->SetWorldRotation(MeshRot);
	}
}