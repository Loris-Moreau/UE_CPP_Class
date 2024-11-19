#include "Controller/GravityGunController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"

#include "Public/Player/Main_Player.h"
#include "Public/Gameplay/GravityGunComponent.h"


UGravityGunController::UGravityGunController()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGravityGunController::SetupInputComponentGravityGunController(TObjectPtr<UInputComponent> InputComponent, class AMain_Player* InCharacter)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent)
		return;
	
	EnhancedInputComponent->BindAction(InputActionTake, ETriggerEvent::Triggered, this, &UGravityGunController::onTakeObjectInputPressed);
	EnhancedInputComponent->BindAction(InputActionThrow, ETriggerEvent::Triggered, this, &UGravityGunController::onThrowObjectInputTriggered);
	
	EnhancedInputComponent->BindAction(InputActionRayGrow, ETriggerEvent::Triggered, this, &UGravityGunController::RaySizeChange);

	
	Character = InCharacter;
	GravityGunComponent = Character->GetComponentByClass<UGravityGunComponent>();
}

void UGravityGunController::BeginPlay()
{
	Super::BeginPlay();
}

void UGravityGunController::onTakeObjectInputPressed()
{
	if (GravityGunComponent.IsValid())
	{
		GravityGunComponent->onTakeObjectInputPressed();
	}
}

void UGravityGunController::onThrowObjectInputTriggered(const struct FInputActionValue& Value)
{
	if (!GravityGunComponent.IsValid())
		return;
	
	const float throwValue = Value.Get<float>();
	
	if( throwValue > 0.f)
	{
		GravityGunComponent->onThrowObjectInputPressed();
	}
	else
	{
		GravityGunComponent->onThrowObjectInputRelease();
	}
}

void UGravityGunController::RaySizeChange()
{
	GravityGunComponent->RaySizeChange();
}
