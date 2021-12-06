// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

#include "Enemy.generated.h"



UCLASS()
class GAMEPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* IdleAnimation;

	//ugrás animáció (flipbook)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* JumpAnimation;

	//sérülés animáció (flipbook)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* HitAnimation;

	//támadás animáció (flipbook)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* AttackAnimation;

	//játékos érzékelés
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Detectplayer)
		class USphereComponent* PlayerDetect;
	//játékos sebzés
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attackplayer)
		class UBoxComponent* EnemyBoxCol;
	//
	//UPROPERTY(VisibleAnywhere, Category = AI)
		//class UPawnSensingComponent* PawnSensingComp;

	void UpdateAnimation();

	void UpdateCharacter();

	bool isattack;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
