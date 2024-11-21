// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PickupSpawnerControllerComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"

#include "Player/Main_Player.h"
#include "Gameplay/PickupSpawnerComponent.h"

UPickupSpawnerControllerComponent::UPickupSpawnerControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPickupSpawnerControllerComponent::SetupInputComponentSpawner(TObjectPtr<UInputComponent> InputComponent,
	AMain_Player* InCharacter)
{
	// Get Enhanced Input Comp
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent)
		return;

	// Bind Action
	EnhancedInputComponent->BindAction(InputActionGetPickupAmount, ETriggerEvent::Triggered, this, &UPickupSpawnerControllerComponent::GetPickupAmount);
	
	EnhancedInputComponent->BindAction(InputActionSpawnNormal, ETriggerEvent::Triggered, this, &UPickupSpawnerControllerComponent::SpawnNormal);
	EnhancedInputComponent->BindAction(InputActionSpawnTakeDestroy, ETriggerEvent::Triggered, this, &UPickupSpawnerControllerComponent::SpawnTakeDestroy);
	EnhancedInputComponent->BindAction(InputActionSpawnThrowDestroy, ETriggerEvent::Triggered, this, &UPickupSpawnerControllerComponent::SpawnThrowDestroy);
	
	Character = InCharacter;
	SpawnerComponent = Character->GetComponentByClass<UPickupSpawnerComponent>();
}

void UPickupSpawnerControllerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPickupSpawnerControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	spawnPos = Character->GetActorLocation() + diff * Character->GetActorForwardVector();
}

void UPickupSpawnerControllerComponent::GetPickupAmount()
{
	if (!SpawnerComponent.IsValid())
		return;
	
	SpawnerComponent->GetPickupAmount();
}

void UPickupSpawnerControllerComponent::SpawnNormal()
{
	if (!SpawnerComponent.IsValid())
		return;
	
	SpawnerComponent->spawnPickup(1, spawnPos);
}

void UPickupSpawnerControllerComponent::SpawnThrowDestroy()
{
	if (!SpawnerComponent.IsValid())
		return;
	
	SpawnerComponent->spawnPickup(2, spawnPos);
}

void UPickupSpawnerControllerComponent::SpawnTakeDestroy()
{
	if (!SpawnerComponent.IsValid())
		return;
	
	SpawnerComponent->spawnPickup(3, spawnPos);
}
