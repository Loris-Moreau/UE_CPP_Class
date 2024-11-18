#include "Controller/MainPlayerController.h"

#include "Controller/GravityGunController.h"
#include "Engine/LocalPlayer.h"
#include "Public/Player/Main_Player.h"
#include "Public/Controller/GravityGunController.h"

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

	// Bind Input Actions
	EnhancedInputComponent->BindAction(InputActionMove, ETriggerEvent::Triggered, this, &AMainPlayerController::MovePlayer);
	EnhancedInputComponent->BindAction(InputActionLook, ETriggerEvent::Triggered, this, &AMainPlayerController::Look);
	EnhancedInputComponent->BindAction(InputActionJump, ETriggerEvent::Triggered, this, &AMainPlayerController::Jump);
}

void AMainPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	// To Bind Only Once
	if (Character)
		return;
	
	Character = Cast<AMain_Player>(InPawn);
	if (!Character)
		return;
	
	GravityGunController = GetComponentByClass<UGravityGunController>();
	if(GravityGunController.IsValid())
		GravityGunController->SetupInputComponentGravityGunController(InputComponent, Character);
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

void AMainPlayerController::Look(const FInputActionValue& Value)
{
	if(!Character)
		return;
	
	const FVector2D LookValue = Value.Get<FVector2D>();
	
	if(LookValue.X != 0.f)
	{
		// Yaw
		Character->AddControllerYawInput(LookValue.X);
	}
	if(LookValue.Y != 0.f)
	{
		// Pitch
		Character->AddControllerPitchInput(LookValue.Y);
	}
}

void AMainPlayerController::Jump()
{
	Character->Jump();
}

void AMainPlayerController::AddYawInput(float Val)
{
	float multipliedValue = Val * mouseSensiX;
	
	Super::AddYawInput(multipliedValue);
}

void AMainPlayerController::AddPitchInput(float Val)
{
	float multipliedValue = Val * mouseSensiY;
	
	Super::AddPitchInput(multipliedValue);
}

