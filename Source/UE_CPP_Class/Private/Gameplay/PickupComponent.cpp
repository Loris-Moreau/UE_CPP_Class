// Fill out your copyright notice in the Description page of Project Settings.


#include "TimerManager.h"
#include "Engine/World.h"
#include "Gameplay/PickUpComponent.h"

UPickUpComponent::UPickUpComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UPickUpComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UPickUpComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ClearDestructionTimer();

	// We NEED to call the endplay after or the clear timer will be called on an empty object
	Super::EndPlay(EndPlayReason);
}

void UPickUpComponent::ClearDestructionTimer()
{
	// Clear timer
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(PickUpDestructionTimerHandle);
}

void UPickUpComponent::StartPickUpDestructionTimer()
{
	// Make sure that we need to destroy it
	if (PickUpStruct.PickUpType == EPickUpType::Normal)
		return;

	// Get time
	const float TimeToDestroy = PickUpStruct.DestructionTimer;

	// Get the timer manager
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(PickUpDestructionTimerHandle);
	TimerManager.SetTimer(PickUpDestructionTimerHandle, this, &UPickUpComponent::DestroyPickUp, TimeToDestroy, false);
}

void UPickUpComponent::DestroyPickUp()
{
	ClearDestructionTimer();

	// Broadcast destruction event
	OnPickUpDestroyed.Broadcast();

	// Destroy the pick up
	GetOwner()->Destroy();
}

EPickUpType UPickUpComponent::GetPickUpType() const
{
	return PickUpStruct.PickUpType;
}
