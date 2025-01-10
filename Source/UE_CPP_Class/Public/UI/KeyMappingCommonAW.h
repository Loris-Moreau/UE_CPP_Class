#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "EnhancedActionKeyMapping.h"
#include "KeyMappingCommonAW.generated.h"

UCLASS()
class UE_CPP_CLASS_API UKeyMappingCommonAW : public UCommonActivatableWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
	TWeakObjectPtr<class AMainPlayerController> playerController = nullptr;
	
	
// KeyBindings
protected:
	UPROPERTY(meta=(BindWidgetOptional))
	class UInputKeySelector* BIND_InputKeySelector= nullptr;
	UPROPERTY(meta=(BindWidgetOptional))
	class UCommonTextBlock* BIND_InputDisplayName = nullptr;

	FName inputName;
	FEnhancedActionKeyMapping displayedKey;
	
public:
	void SetInputName(FName inName);
	void SetInputSelector(struct FEnhancedActionKeyMapping& inActionKeyMapping);
	void SetDisplayName(FText inText);

	void OnKeySelected(FInputChord selectedKey);
};
