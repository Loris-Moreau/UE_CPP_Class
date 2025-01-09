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
	
	// Buttons
protected:
	UPROPERTY(meta=(BindWidgetOptional))
	class UMainCommonButtonBase* BIND_Resume_Button = nullptr;
	
	// Button Functions
protected:
	UFUNCTION()
	void OnResumeClicked();
	
};
