#include "UI/PauseMenuCommonAW.h"

void UPauseMenuCommonAW::NativeConstruct()
{
	Super::NativeConstruct();

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

void UPauseMenuCommonAW::OnResumeClicked()
{
	
}

void UPauseMenuCommonAW::OnRestartClicked()
{
	
}

void UPauseMenuCommonAW::OnOptionsClicked()
{
	
}

void UPauseMenuCommonAW::OnQuitClicked()
{
	
}
