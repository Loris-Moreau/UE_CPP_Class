#include "UI/PauseMenuCommonAW.h"

#include "Kismet/GameplayStatics.h"

void UPauseMenuCommonAW::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Get Player Controller
	playerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
	OpenMenu();
	
	if (BIND_Resume_Button)
	{
		BIND_Resume_Button->OnButtonClicked.AddUniqueDynamic(this, &UPauseMenuCommonAW::OnResumeClicked);
		// Set Focus on this button (First) for Gamepad
		BIND_Resume_Button->SetFocus();
	}
	if (BIND_Restart_Button)
	{
		BIND_Restart_Button->OnButtonClicked.AddUniqueDynamic(this, &UPauseMenuCommonAW::OnRestartClicked);
	}
	if (BIND_Options_Button)
	{
		BIND_Options_Button->OnButtonClicked.AddUniqueDynamic(this, &UPauseMenuCommonAW::OnOptionsClicked);
	}
	if (BIND_Quit_Button)
	{
		BIND_Quit_Button->OnButtonClicked.AddUniqueDynamic(this, &UPauseMenuCommonAW::OnQuitClicked);
	}
}

void UPauseMenuCommonAW::OpenMenu()
{
	SetVisibility(ESlateVisibility::Visible);
	
	if (playerController.IsValid())
	{
		playerController->SetShowMouseCursor(true);
		playerController->SetInputMode(FInputModeUIOnly{});
	}
	
	UGameplayStatics::SetGamePaused(this, true);
}

void UPauseMenuCommonAW::CloseMenu()
{
	SetVisibility(ESlateVisibility::Collapsed);
	RemoveFromParent();
	
	if (playerController.IsValid())
	{
		playerController->SetShowMouseCursor(false);
		playerController->SetInputMode(FInputModeGameOnly{});
	}
	
	UGameplayStatics::SetGamePaused(this, false);
}

void UPauseMenuCommonAW::OnResumeClicked()
{
	CloseMenu();
}

void UPauseMenuCommonAW::OnRestartClicked()
{
	CloseMenu();
	
	// Get Map Name
	FName mapName = FName(UGameplayStatics::GetCurrentLevelName(this));
	// Reopen Level (to restart it)
	UGameplayStatics::OpenLevel(this, mapName);
}

void UPauseMenuCommonAW::OnOptionsClicked()
{
	
}

void UPauseMenuCommonAW::OnQuitClicked()
{
	if (playerController.IsValid())
	{
		UKismetSystemLibrary::QuitGame(this, playerController.Get(), EQuitPreference::Quit, true);
	}
}
