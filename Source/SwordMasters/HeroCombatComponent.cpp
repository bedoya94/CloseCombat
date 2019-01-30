// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordMasters.h"
#include "HeroCombatComponent.h"
#include "SwordMastersCharacter.h"

bool bPressingAttack;

// Sets default values for this component's properties
UHeroCombatComponent::UHeroCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// variable definitions
	bCanAttack = true;
	bAttack = false;
	bCanString = false;
	bIsAttacking = false;
	attackNum = 0;

	//setting variables to pointers
	bCanAttackptr = &bCanAttack;
	bAttackptr = &bAttack;
	bCanStringptr = &bCanString;
	bIsAttackingptr = &bIsAttacking;

	bPressingAttack = false;
}

// Called when the game starts
void UHeroCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	attackNumptr = &attackNum;
	MyOwner = Cast<ASwordMastersCharacter>(GetOwner());
}


// Called every frame
void UHeroCombatComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
	if (MyOwner)
	{
		/*if (bPressingAttack)
			*bAttackptr = true;*/

		if (*bCanAttackptr && *bAttackptr)
		{
			MyOwner->PlayAnimMontage(NormalAttackAnims[*attackNumptr]);

			if (*bCanStringptr)
			{
				//both sintaxis are for incrementing the value of the pointer
				if (*attackNumptr < NormalAttackAnims.Num() - 1)
				{
					++*attackNumptr;
				}
				else
				{
					*attackNumptr = 0;
				}
			}

			*bAttackptr = false;
			*bCanAttackptr = false;
		}		
	}
}

void UHeroCombatComponent::AttackLogic()
{
	bPressingAttack = true;
	*bAttackptr = true;

	if (*bCanAttackptr && *bAttackptr)
	{
		if (!*bCanStringptr)
		{
			//attackNum = FMath::RandRange(0, NormalAttackAnims.Num() - 1);
			*attackNumptr = 0;
		}
		*bCanStringptr = true;
	}
}

void UHeroCombatComponent::AttackEnd()
{
	bPressingAttack = false;
}

void UHeroCombatComponent::StringAttack()
{
	*bCanStringptr = true;
	*bCanAttackptr = true;
}

void UHeroCombatComponent::SwingAttack()
{
	//*bAttackptr = false;
	*bIsAttackingptr = true;
	ThingsHit.Empty();
}

void UHeroCombatComponent::RestAttack()
{
	*bIsAttackingptr = false;
	ThingsHit.Empty();
}

void UHeroCombatComponent::ResetAttack()
{
	*bCanAttackptr = true;
	*bAttackptr = false;
	*bCanStringptr = false;
	*bIsAttackingptr = false;
	*attackNumptr = 0;
	ThingsHit.Empty();
}