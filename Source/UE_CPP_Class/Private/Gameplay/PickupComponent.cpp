// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/PickupComponent.h"
#include "ComponentUtils.h"
#include "TimerManager.h"
#include "Engine/World.h"

#include "Engine/LocalPlayer.h"
#include "Gameplay/PickupSpawnerComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Player/Main_Player.h"
#include "Gameplay/GravityGunComponent.h"

UPickUpComponent::UPickUpComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AMain_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	GravityGunComponent = Character->GetComponentByClass<UGravityGunComponent>();
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

	// Destroy the pickup
	//GetOwner()->Destroy();
	if (!GravityGunComponent.IsValid())
		return;
	
	UE_LOG(LogTemp,Log,TEXT("kawabunga"));
	GravityGunComponent->DestroyPickup(GetOwner());
}

EPickUpType UPickUpComponent::GetPickUpType() const
{
	return PickUpStruct.PickUpType;
}
