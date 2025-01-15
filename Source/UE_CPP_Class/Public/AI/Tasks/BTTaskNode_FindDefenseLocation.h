#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_FindDefenseLocation.generated.h"

UCLASS()
class UE_CPP_CLASS_API UBTTaskNode_FindDefenseLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;

protected:
	TObjectPtr<class AEnemyAIController> EnemyController;
	TObjectPtr<class AEnemy> EnemyCharacter;

	AActor* EnemyGoal = nullptr;
	FVector DefenseLocation = FVector::ZeroVector;
	float DefenseDistFromGoal = -1.0f;

	UPROPERTY(EditAnywhere, Category="Defense")
	struct FBlackboardKeySelector EnemyGoalKey;
	UPROPERTY(EditAnywhere, Category="Defense")
	struct FBlackboardKeySelector DefenseLocationKey;
};
