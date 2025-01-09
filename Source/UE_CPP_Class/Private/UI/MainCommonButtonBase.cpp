#include "UI/MainCommonButtonBase.h"

void UMainCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	if(BIND_MainCommonText)
	{
		BIND_MainCommonText->SetText(buttonText);
	}
}

void UMainCommonButtonBase::NativeOnClicked()
{
	Super::NativeOnClicked();

	OnButtonClicked.Broadcast();
}
