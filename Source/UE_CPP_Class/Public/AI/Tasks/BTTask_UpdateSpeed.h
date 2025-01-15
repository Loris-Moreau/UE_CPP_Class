#pragma once

#include "CoreMinimal.h"
#include "AI/AIEnums.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_UpdateSpeed.generated.h"

UCLASS()
class UE_CPP_CLASS_API UBTTask_UpdateSpeed : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;

protected:
	TObjectPtr<class AEnemyAIController> EnemyController;
	TObjectPtr<class AEnemy> EnemyCharacter;

	float CharacterSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category="Speed")
	TEnumAsByte<EAISpeedType> SpeedType = EAISpeedType::ReachPickup;
	
};
