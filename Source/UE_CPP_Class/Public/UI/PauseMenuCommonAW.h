#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MainCommonButtonBase.h"
#include "Controller/MainPlayerController.h"
#include "PauseMenuCommonAW.generated.h"

UCLASS(Abstract)
class UE_CPP_CLASS_API UPauseMenuCommonAW : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	void OpenMenu();
	void CloseMenu();

	TWeakObjectPtr<class AMainPlayerController> playerController = nullptr;
	
// Buttons
protected:
	UPROPERTY(meta=(BindWidgetOptional))
	class UMainCommonButtonBase* BIND_Resume_Button = nullptr;
	UPROPERTY(meta=(BindWidgetOptional))
	class UMainCommonButtonBase* BIND_Restart_Button = nullptr;
	UPROPERTY(meta=(BindWidgetOptional))
	class UMainCommonButtonBase* BIND_Options_Button = nullptr;
	UPROPERTY(meta=(BindWidgetOptional))
	class UMainCommonButtonBase* BIND_Quit_Button = nullptr;

// Button Functions
protected:
	UFUNCTION()
	void OnResumeClicked();
	UFUNCTION()
	void OnRestartClicked();
	UFUNCTION()
	void OnOptionsClicked();
	UFUNCTION()
	void OnQuitClicked();

	UFUNCTION()
	void OnOptionsMenuClosed(UUserWidget* closedWidget);
	
	UPROPERTY(EditDefaultsOnly, Category="Enhanced Input|Pause")
	TSubclassOf<class UOptionsMenuCommonAW> optionMenuWidget = nullptr;
};
