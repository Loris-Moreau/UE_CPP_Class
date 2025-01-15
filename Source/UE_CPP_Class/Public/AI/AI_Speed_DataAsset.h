#pragma once

#include "CoreMinimal.h"
#include "AI/AIEnums.h"
#include "Engine/DataAsset.h"
#include "AI_Speed_DataAsset.generated.h"

UCLASS()
class UE_CPP_CLASS_API UAI_Speed_DataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Speed Asset")
	float SpeedTypeArray[(uint8)EAISpeedType::MAX];
};
