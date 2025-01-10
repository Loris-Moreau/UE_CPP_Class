#include "UI/OptionsMenuCommonAW.h"

#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputSubsystems.h"
#include "Components/Slider.h"
#include "Components/VerticalBox.h"
#include "Controller/MainPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/KeyMappingCommonAW.h"

void UOptionsMenuCommonAW::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Get Player Controller
	playerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
	if (BIND_Resume_Button)
	{
		BIND_Resume_Button->OnButtonClicked.AddUniqueDynamic(this, &UOptionsMenuCommonAW::OnResumeClicked);
		// Set Focus on this button for Gamepad
		BIND_Resume_Button->SetFocus();
	}

	// Bind Sliders
	if (BIND_X_Slider && playerController.IsValid())
	{
		BIND_X_Slider->OnValueChanged.AddUniqueDynamic(this, &UOptionsMenuCommonAW::ReceiveXSliderValue);
		
		float currentSensi_X = playerController->GetMouseSensitivityX();
		
		BIND_X_Slider->SetValue(currentSensi_X);
		DisplaySensitivity_X(currentSensi_X);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning - No Player Controller in Options Menu"));
	}
	if (BIND_Y_Slider && playerController.IsValid())
	{
		BIND_Y_Slider->OnValueChanged.AddUniqueDynamic(this, &UOptionsMenuCommonAW::ReceiveYSliderValue);
			
		float currentSensi_Y = playerController->GetMouseSensitivityY();
		
		BIND_Y_Slider->SetValue(currentSensi_Y);
		DisplaySensitivity_Y(currentSensi_Y);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning - No Player Controller in Options Menu"));
	}

	displayMappableKeys();
}

void UOptionsMenuCommonAW::OpenMenu()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UOptionsMenuCommonAW::CloseMenu()
{
	SetVisibility(ESlateVisibility::Collapsed);
	RemoveFromParent();
}

void UOptionsMenuCommonAW::OnResumeClicked()
{
	CloseMenu();
}

float UOptionsMenuCommonAW::GetMouseSensitivity_X() const
{
	return mouseSensitivity_X;
}

float UOptionsMenuCommonAW::GetMouseSensitivity_Y() const
{
	return mouseSensitivity_Y;
}

float UOptionsMenuCommonAW::SetMouseSensitivity_X(float inSensitivityX)
{
	if (playerController.IsValid())
	{
		playerController->SetMouseSensitivityX(inSensitivityX);
	}
	
	return mouseSensitivity_X = inSensitivityX;
}

float UOptionsMenuCommonAW::SetMouseSensitivity_Y(float inSensitivityY)
{
	if (playerController.IsValid())
	{
		playerController->SetMouseSensitivityY(inSensitivityY);
	}
	
	return mouseSensitivity_Y = inSensitivityY;
}

void UOptionsMenuCommonAW::ReceiveXSliderValue(float inXValue)
{
	SetMouseSensitivity_X(inXValue);
	DisplaySensitivity_X(inXValue);
}

void UOptionsMenuCommonAW::ReceiveYSliderValue(float inYValue)
{
	SetMouseSensitivity_Y(inYValue);
	DisplaySensitivity_Y(inYValue);
}

void UOptionsMenuCommonAW::DisplaySensitivity_X(float inXValue)
{
	if (BIND_X_Sensitivity_Text)
	{
		// Reduce number to 2 digits (Sanitize)
		float sensiToDisplay = inXValue * 100;
		sensiToDisplay = FMath::RoundToInt(sensiToDisplay) * 0.01f;
		
		// Convert it to Text
		FString sensiString = FString::SanitizeFloat(sensiToDisplay);
		FText displayText = FText::FromString(sensiString);
		
		// and finally display it
		BIND_X_Sensitivity_Text->SetText(displayText);
	}
}

void UOptionsMenuCommonAW::DisplaySensitivity_Y(float inYValue)
{
	if (BIND_Y_Sensitivity_Text)
	{
		// Reduce number to 2 digits (Sanitize)
		float sensiToDisplay = inYValue * 100;
		sensiToDisplay = FMath::RoundToInt(sensiToDisplay) * 0.01f;
		
		// Convert it to Text
		FString sensiString = FString::SanitizeFloat(sensiToDisplay);
		FText displayText = FText::FromString(sensiString);
		
		// and finally display it
		BIND_Y_Sensitivity_Text->SetText(displayText);
	}
}

void UOptionsMenuCommonAW::displayMappableKeys()
{
	if (!playerController.IsValid() || !keyBindingsWidget || !BIND_keyBindingVertBox)
	{
		return;
	}
	// Get Input SubSystem
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	if (!EnhancedInputSubsystem)
	{
		return;
	}
	
	// For each mappable key, create & display a keymapping widget
	TArray<FEnhancedActionKeyMapping> MappableKeysArray = EnhancedInputSubsystem->GetAllPlayerMappableActionKeyMappings();
	for (FEnhancedActionKeyMapping& MappableKey : MappableKeysArray)
	{
		// get key info
		FName keyName = MappableKey.GetMappingName();
		FText keyDisplayName = MappableKey.GetDisplayName();
		
		// create a display infos
		UKeyMappingCommonAW* newKeyWidget =Cast<UKeyMappingCommonAW>(CreateWidget<UKeyMappingCommonAW>(this, keyBindingsWidget));
		if(newKeyWidget)
		{
			newKeyWidget->SetInputName(keyName);
			newKeyWidget->SetDisplayName(keyDisplayName);
			newKeyWidget->SetInputSelector(MappableKey);
		}

		// Place Widget in Vertical Box
		BIND_keyBindingVertBox->AddChild(newKeyWidget);
	}
}
