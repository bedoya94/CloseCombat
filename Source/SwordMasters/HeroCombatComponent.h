// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "HeroCombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDMASTERS_API UHeroCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHeroCombatComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	//this variables manages the attack logic
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool bCanAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool bAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool bCanString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		int32 attackNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool bIsAttacking;

	bool *bCanAttackptr, *bAttackptr, *bIsAttackingptr, *bCanStringptr;
	int32 *attackNumptr;

	//this arrays contains all the animations related to offensive actions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TArray<UAnimMontage*> NormalAttackAnims;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimationAsset* testAttackAnim;

	//this array contains all the actors hitted on the attack
	TArray<AActor*> ThingsHit;

	//All Combat Functions
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void AttackLogic();

	void AttackEnd();
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void StringAttack();
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void SwingAttack();
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void RestAttack();
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void ResetAttack();

	class ASwordMastersCharacter* MyOwner;
};
