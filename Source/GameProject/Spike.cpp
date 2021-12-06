// Fill out your copyright notice in the Description page of Project Settings.


#include "Spike.h"

#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ASpike::ASpike()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	SpikeBoxCol = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SpikeBoxCol->SetBoxExtent(FVector(62.f,102.f,40.f));
	SpikeBoxCol->SetCollisionProfileName("Trigger");
	RootComponent = SpikeBoxCol;

	SpikeBoxCol->OnComponentBeginOverlap.AddDynamic(this, &ASpike::OnCompHit);

	

}

// Called when the game starts or when spawned
void ASpike::BeginPlay()
{
	Super::BeginPlay();
	
	//Castolás: GameProjectCharacter adatainak elérése 
	GameProjectCharacter = Cast<AGameProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	
	
}

// Called every frame
void ASpike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpike::OnCompHit(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
	
	GameProjectCharacter->HealthWidget->Damage(0.2f);
	GameProjectCharacter->isdamaged = true;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("sebzés!"));
	GetWorld()->GetTimerManager().SetTimer(GameProjectCharacter->AttackTimeHandle, [&]() {GameProjectCharacter->isdamaged = false; }, 0.2f, false);
	GameProjectCharacter->SetDeath();

}



