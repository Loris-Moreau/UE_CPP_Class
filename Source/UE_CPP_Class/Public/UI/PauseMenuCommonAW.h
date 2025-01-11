#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "PauseMenuCommonAW.generated.h"

UCLASS(Abstract)
class UE_CPP_CLASS_API UPauseMenuCommonAW : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	void OpenMenu();

	TWeakObjectPtr<class AMainPlayerController> PlayerController = nullptr;

	// Button
protected:
	UPROPERTY(meta = (BindWidgetOptional))
	class UMainCommonButtonBase* BIND_ResumeButton = nullptr;
	UPROPERTY(meta = (BindWidgetOptional))
	class UMainCommonButtonBase* BIND_RestartButton = nullptr;
	UPROPERTY(meta = (BindWidgetOptional))
	class UMainCommonButtonBase* BIND_QuitButton = nullptr;

protected:
	UFUNCTION()
	void OnResumeClicked();
	UFUNCTION()
	void OnRestartClicked();
	UFUNCTION()
	void OnQuitClicked();

	// End of Button


	// Options
protected:
	UPROPERTY(meta = (BindWidgetOptional))
	class UMainCommonButtonBase* BIND_OptionButton = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Option")
	TSubclassOf<class UOptionsMenuCommonAW> OptionMenuWidget = nullptr;

protected:
	UFUNCTION()
	void OnOptionClicked();

	UFUNCTION()
	void OnOptionMenuClosed(UUserWidget* ClosedWidget);

	// End of Options
};
