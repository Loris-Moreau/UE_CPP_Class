#include "Controller/GravityGunController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"


UGravityGunController::UGravityGunController()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGravityGunController::SetupInputComponentGravityGunController(TObjectPtr<UInputComponent> InputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent)
		return;
	
	EnhancedInputComponent->BindAction(InputActionTake, ETriggerEvent::Triggered, this, &UGravityGunController::onTakeObjectInputPressed);
	EnhancedInputComponent->BindAction(InputActionThrow, ETriggerEvent::Triggered, this, &UGravityGunController::onThrowObjectInputTriggered);
}

void UGravityGunController::BeginPlay()
{
	Super::BeginPlay();
}

void UGravityGunController::onTakeObjectInputPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Take"));
}

void UGravityGunController::onThrowObjectInputTriggered(const struct FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Throw"));
}