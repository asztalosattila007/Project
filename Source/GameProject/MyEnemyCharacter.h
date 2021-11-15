// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
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

	//Ugrás animáció (flipbook)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* JumpAnimation;

	//támadás animáció (flipbook)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* AttackAnimation;

	//sérülés animáció (flipbook)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* HitAnimation;

	//halál animáció (flipbook)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* DeathAnimation;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	//Támadás
	void Attack();

	void UpdateCharacter();

	FTimerHandle AttackTimeHandle;
	bool isattack;

public:
	AMyEnemyCharacter();

};