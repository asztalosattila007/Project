// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "MyEnemyCharacter.h"
#include "GameProjectCharacter.generated.h"

class UTextRenderComponent;

/**
 * This class is the default character for GameProject, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */
UCLASS(config=Game)
class AGameProjectCharacter : public APaperCharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;

protected:
	

	virtual void BeginPlay() override;

	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
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

	/** Called for side to side input */
	void MoveRight(float Value);

	//Támadás
	void Attack();  

	void UpdateCharacter();


	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	


	


public:
	AGameProjectCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	//Életerõ (alapértelmezett)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float baseHealth;

	//Életerõ (jelenlegi)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float CurrentHealth;

	//ellenség sebzés
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AttackEnemy)
		class UBoxComponent* PlayerBoxCol;

	/*UFUNCTION()
		virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;*/

	FTimerHandle AttackTimeHandle;
	bool isattack;
	bool isdamaged;
	bool isdead;
	bool canApplyDamage;

	UPROPERTY(EditAnywhere, Category = "Class Types")
		TSubclassOf<UUserWidget> HealthWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Runtime")
		class UHealthBar* HealthWidget;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
	//UFUNCTION()
		//void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void SetDeath();
	
	UPROPERTY(EditAnywhere, Category = "Class Types")
		TSubclassOf<AMyEnemyCharacter> MyEnemyClass;
	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	AMyEnemyCharacter* MyEnemyCharacter;

};
