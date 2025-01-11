#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "OptionsMenuCommonAW.generated.h"


UCLASS(Abstract)
class UE_CPP_CLASS_API UOptionsMenuCommonAW : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	void OpenMenu();

	TWeakObjectPtr<class AMainPlayerController> PlayerController = nullptr;


	// Button
protected:
	UPROPERTY(meta = (BindWidgetOptional))
	class UMainCommonButtonBase* BIND_ExitButton = nullptr;

protected:
	UFUNCTION()
	void OnExitButtonClicked();

	// End of Button


	// Sliders
protected:
	UPROPERTY(meta = (BindWidgetOptional))
	class USlider* BIND_XSlider = nullptr;
	UPROPERTY(meta = (BindWidgetOptional))
	class USlider* BIND_YSlider = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	class UCommonTextBlock* BIND_XDisplay = nullptr;
	UPROPERTY(meta = (BindWidgetOptional))
	class UCommonTextBlock* BIND_YDisplay = nullptr;

protected:
	UFUNCTION()
	void RecieveXSliderValue(float InFloat);
	UFUNCTION()
	void RecieveYSliderValue(float InFloat);

	void DisplaySensitivityX(float InSensitivity);
	void DisplaySensitivityY(float InSensitivity);

	// End of Sliders


	// Keybindings
protected:
	UPROPERTY(meta = (BindWidgetOptional))
	class UVerticalBox* BIND_KeybindingsVerticalBox = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Keybindings")
	TSubclassOf<class UKeyMappingCommonAW> KeybindingsWidget = nullptr;

protected:
	void DisplayMappableKeys();

	// End of Keybindings
};
