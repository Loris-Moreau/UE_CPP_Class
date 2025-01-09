// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MainPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"

#include "Player/Main_Player.h"
#include "Controller/GravityGunController.h"
#include "Controller/PickupSpawnerControllerComponent.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

#include "Kismet/GameplayStatics.h"
#include "Gameplay/Goal.h"
#include "UI/PauseMenuCommonAW.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoal::StaticClass(), GoalArray);
	for (AActor* Goal : GoalArray)
	{
		AGoal* GoalCasted = Cast<AGoal>(Goal);
		if (GoalCasted)
		{
			GoalCasted->OnSendScore.AddUniqueDynamic(this, &AMainPlayerController::OnSendScore);
		}
	}
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind to subsystem
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!EnhancedInputSubsystem)
		return;

	EnhancedInputSubsystem->ClearAllMappings();
	EnhancedInputSubsystem->AddMappingContext(InputMapping, 0);

	// Get Enhanced Input Comp
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent)
		return;
	EnhancedInputComponent->ClearActionBindings();

	// Bind Movements
	EnhancedInputComponent->BindAction(InputActionMove, ETriggerEvent::Triggered, this, &AMainPlayerController::MovePlayer);
	EnhancedInputComponent->BindAction(InputActionLook, ETriggerEvent::Triggered, this, &AMainPlayerController::Look);
	EnhancedInputComponent->BindAction(InputActionJump, ETriggerEvent::Triggered, this, &AMainPlayerController::Jump);

	// Exercice 3
	EnhancedInputComponent->BindAction(InputActionScore, ETriggerEvent::Triggered, this, &AMainPlayerController::OnDisplayScoreInputPressed);

	// Pause
	EnhancedInputComponent->BindAction(InputActionPause, ETriggerEvent::Triggered, this, &AMainPlayerController::OnPausedInputPressed);
}

void AMainPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	// We want to bind only once to the inputs
	if (Character)
		return;

	// Get a ref to our Character
	Character = Cast<AMain_Player>(InPawn);
	if (!Character)
		return;

	GravityGunController = GetComponentByClass<UGravityGunController>();
	if (GravityGunController.IsValid())
	{
		GravityGunController->SetupInputComponentGravityGun(InputComponent, Character);
	}
	// Pickup Spawner
	SpawnerController = GetComponentByClass<UPickupSpawnerControllerComponent>();
	if (SpawnerController.IsValid())
	{
		SpawnerController->SetupInputComponentSpawner(InputComponent, Character);
	}
	
}

void AMainPlayerController::MovePlayer(const FInputActionValue& Value)
{
	if (!Character)
		return;

	// Get movement value
	const FVector2D MoveValue = Value.Get<FVector2D>();

	// Check if the player wants to move in the forward direction
	if (MoveValue.Y != 0.f)
	{
		Character->AddMovementInput(Character->GetActorForwardVector(), MoveValue.Y);
	}

	// Check if the player wants to move in the right direction
	if (MoveValue.X != 0.f)
	{
		Character->AddMovementInput(Character->GetActorRightVector(), MoveValue.X);
	}
}

void AMainPlayerController::Look(const FInputActionValue& Value)
{
	if (!Character)
		return;

	// Get movement value
	const FVector2D MoveValue = Value.Get<FVector2D>();

	if (MoveValue.X != 0.f)
	{
		Character->AddControllerYawInput(MoveValue.X);
	}

	if (MoveValue.Y != 0.f)
	{
		Character->AddControllerPitchInput(MoveValue.Y);
	}
}

void AMainPlayerController::Jump()
{
	if (!Character)
		return;

	Character->Jump();
}

void AMainPlayerController::AddPitchInput(float Value)
{
	float MultipliedValue = Value * MouseSensitivityY;
	Super::AddPitchInput(MultipliedValue);
}

void AMainPlayerController::AddYawInput(float Value)
{
	float MultipliedValue = Value * MouseSensitivityX;
	Super::AddYawInput(MultipliedValue);
}

void AMainPlayerController::OnSendScore(unsigned int Score, FString GoalName)
{
	UE_LOG(LogTemp, Log, TEXT("Timmy %s has a score of %d"), *GoalName, Score);
}

void AMainPlayerController::OnDisplayScoreInputPressed()
{
	for (AActor* Goal : GoalArray)
	{
		AGoal* GoalCasted = Cast<AGoal>(Goal);
		if (GoalCasted)
		{
			GoalCasted->DisplayScore();

			unsigned int PickUpInside = GoalCasted->CountPickUpInside();
			FString GoalName = UKismetSystemLibrary::GetDisplayName(GoalCasted);
			UE_LOG(LogTemp, Log, TEXT("This %s has %d Pickups Inside"), *GoalName, PickUpInside);
		}
	}
}

void AMainPlayerController::OnPausedInputPressed()
{
	if (pauseMenuWidget)
	{
		UPauseMenuCommonAW* currentPauseMenuWidget = Cast<UPauseMenuCommonAW>(CreateWidget<UPauseMenuCommonAW>(this, pauseMenuWidget));

		if (currentPauseMenuWidget)
		{
			currentPauseMenuWidget->AddToViewport(0);
		}
	}
}