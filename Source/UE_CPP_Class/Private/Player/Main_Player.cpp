// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Main_Player.h"

#include "Engine/Engine.h"

AMain_Player::AMain_Player()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void AMain_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMain_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AMain_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

