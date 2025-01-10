#include "UI/KeyMappingCommonAW.h"

#include "CommonTextBlock.h"
#include "Components/InputKeySelector.h"
#include "Controller/MainPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UKeyMappingCommonAW::NativeConstruct()
{
	Super::NativeConstruct();

	
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
	
}