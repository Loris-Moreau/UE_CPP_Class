// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/GravityGunController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"

#include "Player/Main_Player.h"
#include "Gameplay/GravityGunComponent.h"

UGravityGunController::UGravityGunController()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UGravityGunController::BeginPlay()
{
	Super::BeginPlay();
	
}

void UGravityGunController::SetupInputComponentGravityGun(TObjectPtr<class UInputComponent> InputComponent, class AMain_Player* InCharacter)
{
	// Get Enhanced Input Comp
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent)
		return;
	
	// Bind Actions
	EnhancedInputComponent->BindAction(InputActionTakeObject, ETriggerEvent::Triggered, this, &UGravityGunController::OnTakeObjectInputPressed);
	EnhancedInputComponent->BindAction(InputActionThrowObject, ETriggerEvent::Triggered, this, &UGravityGunController::OnThrowObjectInputTriggered);

	Character = InCharacter;
	GravityGunComponent = Character->GetComponentByClass<UGravityGunComponent>();

	// Exercice 1
	EnhancedInputComponent->BindAction(InputActionRaycastSize, ETriggerEvent::Triggered, this, &UGravityGunController::OnUpdateRaycastSizeInputTriggered);

	// Exercice 2
	EnhancedInputComponent->BindAction(InputActionThrowForceMultiplier, ETriggerEvent::Triggered, this, &UGravityGunController::OnThrowForceMultiplierInputPressed);

	
	EnhancedInputComponent->BindAction(InputActionDestroyPickup, ETriggerEvent::Triggered, this, &UGravityGunController::DestroyPickup);
}

void UGravityGunController::OnTakeObjectInputPressed()
{
	if (GravityGunComponent.IsValid())
	{
		GravityGunComponent->OnTakeObjectInputPressed();
	}
}

void UGravityGunController::OnThrowObjectInputTriggered(const FInputActionValue& Value)
{
	if (!GravityGunComponent.IsValid())
		return;

	const float ThrowValue = Value.Get<float>();
	if (ThrowValue > 0.f)
	{
		GravityGunComponent->OnThrowObjectInputPressed();
	}
	else
	{
		GravityGunComponent->OnThrowObjectInputReleased();
	}
}

void UGravityGunController::OnUpdateRaycastSizeInputTriggered(const FInputActionValue& Value)
{
	if (!GravityGunComponent.IsValid())
		return;

	const float FloatValue = Value.Get<float>();
	if (FloatValue >= 0.f)
	{
		GravityGunComponent->OnIncreaseRaycastSize();
	}
	else
	{
		GravityGunComponent->OnDecreaseRaycastSize();
	}
}

void UGravityGunController::OnThrowForceMultiplierInputPressed()
{
	if (!GravityGunComponent.IsValid())
		return;

	GravityGunComponent->OnThrowForceMultiplierInputPressed();
}

void UGravityGunController::DestroyPickup()
{
	GravityGunComponent->DestroyPickup();
}
