#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "AI/AIEnums.h"
#include "AITargetPoint.generated.h"

UCLASS(Abstract)
class UE_CPP_CLASS_API AAITargetPoint : public ATargetPoint
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="AI Target Point")
	EAIBehaviourType TargetBehaviourType = EAIBehaviourType::None;

public:
	EAIBehaviourType GetTargetBehaviorType() const;
};
