#pragma once

#include "CoreMinimal.h"
#include "AI/AIEnums.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_TargetAIBehavior.generated.h"

UCLASS(MinimalAPI)
class UEnvQueryTest_TargetAIBehavior : public UEnvQueryTest
{
	GENERATED_UCLASS_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="Goal")
	EAIBehaviourType TargetBehaviorType = EAIBehaviourType::None;

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
	
	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
	
};
