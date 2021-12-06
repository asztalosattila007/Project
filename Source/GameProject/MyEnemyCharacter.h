// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

#include "MyEnemyCharacter.generated.h"

/**
 * 
 */


UCLASS(config = Game)
class GAMEPROJECT_API AMyEnemyCharacter : public APaperCharacter
{
	GENERATED_BODY()


	virtual void Tick(float DeltaSeconds) override;
protected:
	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* IdleAnimation;

	//Ugr�s anim�ci� (flipbook)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* JumpAnimation;

	//t�mad�s anim�ci� (flipbook)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* AttackAnimation;

	//s�r�l�s anim�ci� (flipbook)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* HitAnimation;

	//hal�l anim�ci� (flipbook)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* DeathAnimation;
	
	
	
	//j�t�kos sebz�s
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attackplayer)
		class UBoxComponent* EnemyBoxCol;
	//j�t�kos �rz�kel�s
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Detectplayer)
		class USphereComponent* PlayerDetect;
	

	

	//EditAnywhere,VisibleAnywhere

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	//T�mad�s
	//void Attack();

	void UpdateCharacter();

	void IsDead(); //igaz ha az enemyhealth=0.0;
	
	bool isattack;
	
	

public:
	AMyEnemyCharacter();
	bool isdamaged;
	bool isdead;
	FTimerHandle AttackTimeHandle;
	float enemyHealth;
};