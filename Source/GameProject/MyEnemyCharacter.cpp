// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemyCharacter.h"

#include "GameProjectCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"

#include "GameFramework/MovementComponent.h"



//////////////////////////////////////////////////////////////////////////
// AMyEnemyCharacter

AMyEnemyCharacter::AMyEnemyCharacter()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);


	// Prevent all automatic rotation behavior on the camera, character, and camera component
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = false;

	// 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
	// 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	// 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
	// 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	// 	TextComponent->SetupAttachment(RootComponent);

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;


	isattack = false; //támadás alap érték
}

//////////////////////////////////////////////////////////////////////////
// Animation

void AMyEnemyCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	if (isattack == true) {
		GetSprite()->SetFlipbook(AttackAnimation);
	}
	else if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
	//támadás ugrás közben  
	if (isattack == true && GetMovementComponent()->IsFalling())
	{
		GetSprite()->SetFlipbook(AttackAnimation);
	}
	//ugrás animációra átváltás
	else if (GetMovementComponent()->IsFalling()) {
		GetSprite()->SetFlipbook(JumpAnimation);
	}

	//else {
	//	GetSprite()->SetFlipbook(DesiredAnimation);
	//}
}

void AMyEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateCharacter();
}



void AMyEnemyCharacter::Attack()
{
	//támadás animáció lejátszás
	//érzékeljen minden ellenfélt a támadás sugarában.
	//És sebezze meg azokat.
	isattack = true;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Tamadas!"));
	GetWorld()->GetTimerManager().SetTimer(AttackTimeHandle, [&]() {isattack = false; }, 0.5f, false);


}

void AMyEnemyCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}


