// Copyright Epic Games, Inc. All Rights Reserved.

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
#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"
#include "HealthBar.h"
#include "Blueprint/UserWidget.h"


DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// AGameProjectCharacter

void AGameProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("gamemode beginply"));

	if (HealthWidgetClass) {
		HealthWidget = CreateWidget<UHealthBar>(GetWorld(), HealthWidgetClass);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("valid widgetclass"));

		if (HealthWidget) {
			HealthWidget->AddToViewport();
			HealthWidget->SetVisibility(ESlateVisibility::Visible);
			HealthWidget->Updatewidget();
			
		}
	}

	//castolás: ellenség karakter adatai
	//MyEnemyCharacter = Cast<AMyEnemyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)); 
	MyEnemyCharacter = Cast<AMyEnemyCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), MyEnemyClass));
}

AGameProjectCharacter::AGameProjectCharacter()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	//box component
	PlayerBoxCol = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	PlayerBoxCol->AttachTo(RootComponent);
	PlayerBoxCol->OnComponentBeginOverlap.AddDynamic(this, &AGameProjectCharacter::OnOverlapBegin);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->SetUsingAbsoluteRotation(true);
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
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
	isdamaged = false; //sérülés alap érték
	isdead = false;

	//Életerõ: basehealth 100%-ra állítva, Currenthealth alapérték megadva.
	baseHealth = 1.0f;
	CurrentHealth = baseHealth;

}

//void AGameProjectCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
//{
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("I Hit: spike")));
//	HealthWidget->Damage(0.5f);
//	isdamaged = true;
//	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("sebzés!"));
//	GetWorld()->GetTimerManager().SetTimer(AttackTimeHandle, [&]() {isdamaged = false; }, 0.2f, false);
//	if (HealthWidget->isDead()) {
//		isdead = true;
//		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Meghalt!"));
//		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
//		DisableInput(Cast<APlayerController>(this));
//		//EnableInput(Cast<APlayerController>(this));
//	}
//	
//}


//////////////////////////////////////////////////////////////////////////
// Animation

void AGameProjectCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	if (isdead == true) {
		GetSprite()->SetFlipbook(DeathAnimation);
	}
	else if (isdamaged == true) {
		GetSprite()->SetFlipbook(HitAnimation);
	}
	else if (isattack == true) {
		GetSprite()->SetFlipbook(AttackAnimation);
	}
	else if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
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

void AGameProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateCharacter();	
}


//////////////////////////////////////////////////////////////////////////
// Input

void AGameProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGameProjectCharacter::MoveRight);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AGameProjectCharacter::Attack);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGameProjectCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGameProjectCharacter::TouchStopped);
}

void AGameProjectCharacter::MoveRight(float Value)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void AGameProjectCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Jump on any touch
	Jump();
}

void AGameProjectCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Cease jumping once touch stopped
	StopJumping();
}

void AGameProjectCharacter::Attack()
{
	//támadás animáció lejátszás
	//érzékeljen minden ellenfélt a támadás sugarában.
	//És sebezze meg azokat.
	isattack = true;
	 //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Tamadas!"));
	 GetWorld()->GetTimerManager().SetTimer(AttackTimeHandle, [&]() {isattack = false; }, 0.6f, false);
	 

}


void AGameProjectCharacter::UpdateCharacter()
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

void AGameProjectCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)

{
	
		if (isattack == true) {
			if (OtherActor && (OtherActor != this) && OtherActor == MyEnemyCharacter) {
				if (MyEnemyClass) {
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
					MyEnemyCharacter->isdamaged = true;
					GetWorld()->GetTimerManager().SetTimer(MyEnemyCharacter->AttackTimeHandle, [&]() {MyEnemyCharacter->isdamaged = false; }, 0.6f, false);
					MyEnemyCharacter->enemyHealth -=  0.5;
				}
			}
		}
	
}

//void AGameProjectCharacter::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	/*if (isattack == true) {
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
//	}*/
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
//}

void AGameProjectCharacter::SetDeath()
{
	if (HealthWidget->isDead()) {
				isdead = true;
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Meghalt!"));
				APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
				DisableInput(Cast<APlayerController>(this));
				//EnableInput(Cast<APlayerController>(this));
			}
}
