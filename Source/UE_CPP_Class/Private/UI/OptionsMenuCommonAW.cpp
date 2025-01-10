#include "UI/OptionsMenuCommonAW.h"


void UOptionsMenuCommonAW::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (BIND_Resume_Button)
	{
		BIND_Resume_Button->OnButtonClicked.AddUniqueDynamic(this, &UOptionsMenuCommonAW::OnResumeClicked);
		// Set Focus on this button for Gamepad
		BIND_Resume_Button->SetFocus();
	}
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
