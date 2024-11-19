// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Goal.h"

// Sets default values
AGoal::AGoal(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, "CollisionBox");
	if (CollisionBox)
	{
		SetRootComponent(CollisionBox);
	}
}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();
	
}
