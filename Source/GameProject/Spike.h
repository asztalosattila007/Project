// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameProjectCharacter.h"
#include "HealthBar.h"

#include "Spike.generated.h"



UCLASS(Abstract)
class GAMEPROJECT_API ASpike : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpike();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* SpikeBoxCol;
	
	UFUNCTION()
		void OnCompHit(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	

	//a játékos adatainak hozzáféréséhez (castoláshoz)
	AGameProjectCharacter* GameProjectCharacter;

	
};
