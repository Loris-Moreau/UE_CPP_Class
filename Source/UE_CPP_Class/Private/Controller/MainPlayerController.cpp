#include "Controller/MainPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Public/Player/Main_Player.h"

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

void AMainPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	Character = Cast<AMain_Player>(InPawn);
}

void AMainPlayerController::MovePlayer(const FInputActionValue& Value)
{
	if(!Character)
		return;
	
	const FVector2D MoveValue = Value.Get<FVector2D>();

	// Check if Player Move Forward
	if(MoveValue.Y != 0.f )
	{
		Character->AddMovementInput(Character->GetActorForwardVector(), MoveValue.Y);
	}
	// Check Right
	if(MoveValue.X != 0.f )
	{
		Character->AddMovementInput(Character->GetActorRightVector(), MoveValue.X);
	}
}
