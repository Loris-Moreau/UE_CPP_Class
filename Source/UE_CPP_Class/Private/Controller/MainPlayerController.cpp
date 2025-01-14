#include "Controller/MainPlayerController.h"

#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "PlayerMappableKeySettings.h"
#include "Controller/GravityGunController.h"
#include "Controller/PickupSpawnerControllerComponent.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/GameUserSettings.h"
#include "Gameplay/Goal.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Main_Player.h"
#include "Save/OptionSaveGame.h"
#include "UI/KeyMappingCommonAW.h"
#include "UI/PauseMenuCommonAW.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Load Save Data
	LoadOptionData();

	// Goal & Scoring
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
	EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
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
	EnhancedInputComponent->BindAction(InputActionPause, ETriggerEvent::Triggered, this, &AMainPlayerController::OnPauseInputPressed);

	// KeyBindings (Tell Unreal we're going to update the key in the Input Mapping Context)
	EnhancedInputUserSettings = EnhancedInputSubsystem->GetUserSettings();
	if (EnhancedInputUserSettings)
	{
		EnhancedInputUserSettings->RegisterInputMappingContext(InputMapping);
	}
	
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

float AMainPlayerController::GetMouseSensitivityX() const
{
	return MouseSensitivityX;
}

float AMainPlayerController::GetMouseSensitivityY() const
{
	return MouseSensitivityY;
}

void AMainPlayerController::SetMouseSensitivityX(float inValue)
{
	MouseSensitivityX = inValue;
	
	SaveOptionData();
}

void AMainPlayerController::SetMouseSensitivityY(float inValue)
{
	MouseSensitivityY = inValue;
	
	SaveOptionData();
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

void AMainPlayerController::OnPauseInputPressed()
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

void AMainPlayerController::OnUpdateBindedKey(FName InputName, FKey NewKey)
{
	if (!EnhancedInputUserSettings)
	{
		return;
	}

	// Prepare new key
	FMapPlayerKeyArgs InArgs;
	InArgs.NewKey = NewKey;
	InArgs.MappingName = InputName;
	InArgs.Slot = EPlayerMappableKeySlot::First;
	FGameplayTagContainer FailureReason;

	// Update the key
	EnhancedInputUserSettings->MapPlayerKey(InArgs, FailureReason);


	// Save the inputs
	UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
	GameUserSettings->ApplySettings(true);

	EnhancedInputUserSettings->ApplySettings();
	EnhancedInputUserSettings->SaveSettings();
}

void AMainPlayerController::OnResetBindedKey(FName InputName, FEnhancedActionKeyMapping& ActionKeyMapping, UKeyMappingCommonAW* InWidget)
{
	if (!EnhancedInputUserSettings)
	{
		return;
	}

	// Prepare the current
	FName KeyName = ActionKeyMapping.GetPlayerMappableKeySettings()->Name;
	FMapPlayerKeyArgs CurrentKeyArgs;
	CurrentKeyArgs.MappingName = KeyName;
	CurrentKeyArgs.NewKey = ActionKeyMapping.Key;
	CurrentKeyArgs.Slot = EPlayerMappableKeySlot::First;

	// Get the key profile to find the default data
	UEnhancedPlayerMappableKeyProfile* KeyProfile = EnhancedInputUserSettings->GetCurrentKeyProfile();
	if (!KeyProfile)
	{
		return;
	}

	// Get Default Key
	FPlayerKeyMapping* DefaultKeyMapping = KeyProfile->FindKeyMapping(CurrentKeyArgs);
	FKey DefaultKey = DefaultKeyMapping->GetDefaultKey();

	// Set back to Default Key
	OnUpdateBindedKey(InputName, DefaultKey);

	// Update the display
	// In the argument of the func we already have the required struct.
	// So we just need to send it back with the right key.
	ActionKeyMapping.Key = DefaultKey;
	InWidget->SetInputSelector(ActionKeyMapping);
}

void AMainPlayerController::SaveOptionData()
{
	// Try to Get Save File
	UOptionSaveGame* OptionSaveGame = nullptr;
	if(UGameplayStatics::DoesSaveGameExist(SaveOptionSlotName, 0))
	{
		OptionSaveGame =
			Cast<UOptionSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveOptionSlotName, 0));
	}
	// if it doesnt exist create it
	else
	{
		OptionSaveGame =
			Cast<UOptionSaveGame>(UGameplayStatics::CreateSaveGameObject(UOptionSaveGame::StaticClass()));
	}
	// Update Data in Save File
	OptionSaveGame->SetMouseSensitivityX(MouseSensitivityX);
	OptionSaveGame->SetMouseSensitivityY(MouseSensitivityY);

	// Save Data
	UGameplayStatics::SaveGameToSlot(OptionSaveGame, SaveOptionSlotName, 0);
}

void AMainPlayerController::LoadOptionData()
{
	// Try to Get Save File
	UOptionSaveGame* OptionSaveGame = nullptr;
	if(UGameplayStatics::DoesSaveGameExist(SaveOptionSlotName, 0))
	{
		OptionSaveGame =
			Cast<UOptionSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveOptionSlotName, 0));
	}
	// if it doesn't exist we have nothing to load
	else
	{
		return;
	}

	// Load Data
	MouseSensitivityX = OptionSaveGame->GetMouseSensitivityX();
	MouseSensitivityY = OptionSaveGame->GetMouseSensitivityY();
}
