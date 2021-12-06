// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"
#include "Components/ProgressBar.h"
#include "GameProjectCharacter.h"

void UHealthBar::Updatewidget()
{
	
	HealthBar->SetPercent(1.0);
}

void UHealthBar::Damage(float value)
{
	HealthBar->SetPercent(HealthBar->Percent-value);
}

bool UHealthBar::isDead()
{
	return HealthBar->Percent<=0;
}
