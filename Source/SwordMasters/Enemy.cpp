// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordMasters.h"
#include "Enemy.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<ASwordMastersCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

// Called every frame
void AEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	direction = GetDirectionToPlayer();
	distance = GetDistanceToPlayer();
}

float AEnemy::GetDirectionToPlayer()
{
	float _dir = 0;
	FVector PlayerLocation = Player->GetActorLocation();
	FVector MyLocation = this->GetActorLocation();

	FVector Direction = MyLocation - PlayerLocation;
	Direction.Normalize();
	_dir = FVector::DotProduct(Direction, Player->GetActorForwardVector());
	
	return _dir;
}

float AEnemy::GetDistanceToPlayer()
{
	float _dist = 0;

	FVector PlayerLocation = Player->GetActorLocation();
	FVector MyLocation = this->GetActorLocation();

	_dist = FVector::Dist(MyLocation, PlayerLocation);
	return _dist;
}

bool AEnemy::IsOnPlayerRange(float _dir, float _dist)
{
	bool _bInRange = false;

	return _bInRange;
}