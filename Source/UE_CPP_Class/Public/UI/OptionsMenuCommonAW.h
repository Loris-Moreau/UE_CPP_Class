#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UI/MainCommonButtonBase.h"
#include "OptionsMenuCommonAW.generated.h"

UCLASS(Abstract)
class UE_CPP_CLASS_API UOptionsMenuCommonAW : public UCommonActivatableWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	void OpenMenu();
	void CloseMenu();
	
	TWeakObjectPtr<class AMainPlayerController> playerController = nullptr;
	
	// Buttons
protected:
	UPROPERTY(meta=(BindWidgetOptional))
	class UMainCommonButtonBase* BIND_Resume_Button = nullptr;
	
	// Button Functions
protected:
	UFUNCTION()
	void OnResumeClicked();
	
// Sensitivity
public:
	//Getter
	float GetMouseSensitivity_X() const;
	float GetMouseSensitivity_Y() const;
	//Setter
	float SetMouseSensitivity_X(float inSensitivityX);
	float SetMouseSensitivity_Y(float inSensitivityY);

	float mouseSensitivity_X = 0.75f;
	float mouseSensitivity_Y = 0.75f;

// Sliders
protected:
	UPROPERTY(meta=(BindWidgetOptional))
	class USlider* BIND_X_Slider = nullptr;
	UPROPERTY(meta=(BindWidgetOptional))
	class USlider* BIND_Y_Slider = nullptr;
	
	UPROPERTY(meta=(BindWidgetOptional))
	class UCommonTextBlock* BIND_X_Sensitivity_Text = nullptr;
	UPROPERTY(meta=(BindWidgetOptional))
	class UCommonTextBlock* BIND_Y_Sensitivity_Text = nullptr;

	UFUNCTION()
	void ReceiveXSliderValue(float inXValue);
	UFUNCTION()
	void ReceiveYSliderValue(float inYValue);

	void DisplaySensitivity_X(float inXValue);
	void DisplaySensitivity_Y(float inYValue);
	
// KeyBindings
protected:
	UPROPERTY(meta=(BindWidgetOptional))
	class UVerticalBox* BIND_keyBindingVertBox = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Enhanced Input|Bindings")
	TSubclassOf<class UKeyMappingCommonAW> keyBindingsWidget = nullptr;

	void displayMappableKeys();
};
