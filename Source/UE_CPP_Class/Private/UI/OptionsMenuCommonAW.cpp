#include "UI/OptionsMenuCommonAW.h"
#include "UI/MainCommonButtonBase.h"
#include "UI/KeyMappingCommonAW.h"
#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"
#include "CommonTextBlock.h"
#include "Components/VerticalBox.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedActionKeyMapping.h"
#include "Controller/MainPlayerController.h"

void UOptionsMenuCommonAW::NativeConstruct()
{
	Super::NativeConstruct();

	// Get player controller
	PlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!PlayerController.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning - No Player Controller Found in Option Menu"));
	}

	OpenMenu();

	// Bind exit button
	if (BIND_ExitButton)
	{
		BIND_ExitButton->OnButtonClicked.AddUniqueDynamic(this, &UOptionsMenuCommonAW::OnExitButtonClicked);
	}

	// Bind Sliders and display current value
	if(BIND_XSlider && PlayerController.IsValid())
	{
		// Bind to value changed
		BIND_XSlider->OnValueChanged.AddUniqueDynamic(this, &UOptionsMenuCommonAW::RecieveXSliderValue);

		// Display the current sensitivity
		float CurrentSensitivityX = PlayerController->GetMouseSensitivityX();
		BIND_XSlider->SetValue(CurrentSensitivityX);
	}

	if (BIND_YSlider && PlayerController.IsValid())
	{
		// Bind to value changed
		BIND_YSlider->OnValueChanged.AddUniqueDynamic(this, &UOptionsMenuCommonAW::RecieveYSliderValue);

		// Display the current sensitivity
		float CurrentSensitivityY = PlayerController->GetMouseSensitivityY();
		BIND_YSlider->SetValue(CurrentSensitivityY);
	}

	// Key Mappings
	DisplayMappableKeys();
}

void UOptionsMenuCommonAW::OpenMenu()
{
	// Display the Widget
	SetVisibility(ESlateVisibility::Visible);
}

void UOptionsMenuCommonAW::OnExitButtonClicked()
{
	// Display the Widget
	SetVisibility(ESlateVisibility::Collapsed);
	RemoveFromParent();
}

void UOptionsMenuCommonAW::RecieveXSliderValue(float InFloat)
{
	// Update and display the new sensitivity
	if (PlayerController.IsValid())
	{
		PlayerController->SetMouseSensitivityX(InFloat);
		DisplaySensitivityX(InFloat);
	}
}

void UOptionsMenuCommonAW::RecieveYSliderValue(float InFloat)
{
	// Update and display the new sensitivity
	if (PlayerController.IsValid())
	{
		PlayerController->SetMouseSensitivityY(InFloat);
		DisplaySensitivityY(InFloat);
	}
}

void UOptionsMenuCommonAW::DisplaySensitivityX(float InSensitivity)
{
	if (BIND_XDisplay)
	{
		// We need to "sanitize" the number aka reduce to 2 digits
		float SensitivityToDisplay = InSensitivity * 100.f; // 2.1325 -> 213.25
		SensitivityToDisplay = FMath::RoundToInt(SensitivityToDisplay) * 0.01f; // 213.25 -> 2.13000
		FString SensitivityString = FString::SanitizeFloat(SensitivityToDisplay); // 2.13000 -> 2.13
		FText SensitivyText = FText::FromString(SensitivityString);

		// Display it
		BIND_XDisplay->SetText(SensitivyText);
	}
}

void UOptionsMenuCommonAW::DisplaySensitivityY(float InSensitivity)
{
	if (BIND_YDisplay)
	{
		// We need to "sanitize" the number aka reduce to 2 digits
		float SensitivityToDisplay = InSensitivity * 100.f; // 2.1325 -> 213.25
		SensitivityToDisplay = FMath::RoundToInt(SensitivityToDisplay) * 0.01f; // 213.25 -> 2.13000
		FString SensitivityString = FString::SanitizeFloat(SensitivityToDisplay); // 2.13000 -> 2.13
		FText SensitivyText = FText::FromString(SensitivityString);

		// Display it
		BIND_YDisplay->SetText(SensitivyText);
	}
}

void UOptionsMenuCommonAW::DisplayMappableKeys()
{
	if (!PlayerController.IsValid() || !KeybindingsWidget || !BIND_KeybindingsVerticalBox)
	{
		return;
	}

	// Get Enhanced Input subsystem
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = 
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!EnhancedInputSubsystem)
	{
		return;
	}

	// For each mappable key, create and display a keymapping widget
	TArray<FEnhancedActionKeyMapping> MappableKeyArray = EnhancedInputSubsystem->GetAllPlayerMappableActionKeyMappings();
	for (FEnhancedActionKeyMapping& MappableKey : MappableKeyArray)
	{
		// Get key infos
		FName KeyName = MappableKey.GetMappingName();
		FText KeyDisplayName = MappableKey.GetDisplayName();
		
		// Create and display infos
		UKeyMappingCommonAW* NewKeyWidget = 
			Cast<UKeyMappingCommonAW>(CreateWidget<UKeyMappingCommonAW>(this, KeybindingsWidget));
		if (NewKeyWidget)
		{
			NewKeyWidget->SetInputName(KeyName);
			NewKeyWidget->SetDisplayName(KeyDisplayName);
			NewKeyWidget->SetInputSelector(MappableKey);
		}

		// Place Widget in Vertical Box
		BIND_KeybindingsVerticalBox->AddChild(NewKeyWidget);
	}
}
