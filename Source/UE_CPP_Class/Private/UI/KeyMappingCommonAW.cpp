#include "UI/KeyMappingCommonAW.h"

#include "CommonTextBlock.h"
#include "Components/InputKeySelector.h"
#include "Controller/MainPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MainCommonButtonBase.h"

void UKeyMappingCommonAW::NativeConstruct()
{
	Super::NativeConstruct();

	if(BIND_InputKeySelector)
	{
		BIND_InputKeySelector->OnKeySelected.AddUniqueDynamic(this, &UKeyMappingCommonAW::OnKeySelected);
	}
	if(BIND_ResetButton)
	{
		BIND_ResetButton->OnButtonClicked.AddUniqueDynamic(this, &UKeyMappingCommonAW::OnResetButtonClicked);
	}
}

void UKeyMappingCommonAW::SetInputName(FName inName)
{
	inputName = inName;
}

void UKeyMappingCommonAW::SetInputSelector(FEnhancedActionKeyMapping& inActionKeyMapping)
{
	if (BIND_InputKeySelector)
	{
		displayedKey = inActionKeyMapping;
		BIND_InputKeySelector->SetSelectedKey(displayedKey.Key);
	}
}

void UKeyMappingCommonAW::SetDisplayName(FText inText)
{
	if (BIND_InputDisplayName)
	{
		BIND_InputDisplayName->SetText(inText);
	}
}

void UKeyMappingCommonAW::OnKeySelected(FInputChord selectedKey)
{
	// Get Player Controller
	playerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if(!playerController.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning - No Player Controller in Options Menu"));
	}

	//Update Keys
	playerController->OnUpdateBindedKey(inputName, selectedKey.Key);
}

void UKeyMappingCommonAW::OnResetButtonClicked()
{
	if(!playerController.IsValid())
	{
		return;
	}
	
	playerController->OnResetButtonClicked(inputName, displayedKey, this);
}
