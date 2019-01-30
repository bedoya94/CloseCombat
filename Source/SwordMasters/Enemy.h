// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "SwordMastersCharacter.h"
#include "Enemy.generated.h"

UCLASS()
class SWORDMASTERS_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	virtual void PostInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	ASwordMastersCharacter* Player;

	UPROPERTY(EditAnywhere)
		float direction;
		float distance;

	float GetDirectionToPlayer();
	float GetDistanceToPlayer();
	bool IsOnPlayerRange(float _dir, float _dist);
	
	
};
