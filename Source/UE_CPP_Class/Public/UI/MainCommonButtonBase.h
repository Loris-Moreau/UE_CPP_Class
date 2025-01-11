#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "MainCommonButtonBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonCliked);

UCLASS(Abstract)
class UE_CPP_CLASS_API UMainCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

// pre construct (in editor)
protected:
	virtual void NativePreConstruct() override;
	
// Text
protected:
	UPROPERTY(EditAnywhere, Category="Main Button|Text")
	FText buttonText = FText();

	UPROPERTY(meta = (BindWidgetOptional))
	class UCommonTextBlock* BIND_MainCommonText = nullptr;
	
// Event
public:
	FOnButtonCliked OnButtonClicked;
	
protected:
	virtual void NativeOnClicked() override;
};
