#include "Controller/MainPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind to subsystem
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!EnhancedInputSubsystem)
		return;

	EnhancedInputSubsystem->ClearAllMappings();
	EnhancedInputSubsystem->AddMappingContext(InputMapping, 0);

	// Get Enhanced input comp
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent)
		return;
	EnhancedInputComponent->ClearActionBindings();

	// Bind Movements
	EnhancedInputComponent->BindAction(InputActionMove, ETriggerEvent::Triggered, this, &AMainPlayerController::MovePlayer);
}

void AMainPlayerController::MovePlayer(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("MOVE PLAYER"));
}