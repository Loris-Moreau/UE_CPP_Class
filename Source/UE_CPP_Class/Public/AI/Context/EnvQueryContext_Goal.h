#pragma once

#include "CoreMinimal.h"
#include "AI/AIEnums.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "GameFramework/Actor.h"
#include "EnvQueryContext_Goal.generated.h"

UCLASS(Abstract, Blueprintable, MinimalAPI)
class UEnvQueryContext_Goal : public UEnvQueryContext
{
	GENERATED_BODY()
	
public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
	
	UPROPERTY(EditAnywhere, Category="Goal")
	TSubclassOf<AActor> GoalBP = nullptr;
	UPROPERTY(EditAnywhere, Category="Goal")
	EAIBehaviourType GoalBehaviorType = EAIBehaviourType::None;
};
