// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Paper2D/Classes/PaperSpriteComponent.h>
#include "GameFramework/MovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);


	// Prevent all automatic rotation behavior on character;
	GetCharacterMovement()->bOrientRotationToMovement = false;


 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	isattack = false;

	PlayerDetect = CreateDefaultSubobject<USphereComponent>(TEXT("Attack collision"));
	PlayerDetect->InitSphereRadius(400.f);
	PlayerDetect->SetCollisionProfileName("Eyesight");
	RootComponent = PlayerDetect;

	EnemyBoxCol = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	EnemyBoxCol->SetBoxExtent(FVector(62.f, 102.f, 40.f));
	EnemyBoxCol->SetCollisionProfileName("Trigger");
	EnemyBoxCol->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// Are we moving or standing still?
	//UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	//if (isattack == true) {
	//	GetSprite()->SetFlipbook(AttackAnimation);
	//}
	//else if (GetSprite()->GetFlipbook() != DesiredAnimation)
	//{
	//	GetSprite()->SetFlipbook(DesiredAnimation);
	//}
	////támadás ugrás közben  
	//if (isattack == true && GetMovementComponent()->IsFalling())
	//{
	//	GetSprite()->SetFlipbook(AttackAnimation);
	//}
	////ugrás animációra átváltás
	//else if (GetMovementComponent()->IsFalling()) {
	//	GetSprite()->SetFlipbook(JumpAnimation);
	//}

}

void AEnemy::UpdateCharacter()
{

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

