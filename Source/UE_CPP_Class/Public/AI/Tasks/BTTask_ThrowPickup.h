#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Gameplay/GravityGunComponent.h"
#include "BTTask_ThrowPickup.generated.h"

UCLASS()
class UE_CPP_CLASS_API UBTTask_ThrowPickup : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;

protected:
	TObjectPtr<class AEnemyAIController> EnemyController;
	TObjectPtr<class AEnemy> EnemyCharacter;

	class UGravityGunComponent* GravityGunComp = nullptr;
	AActor* PlayerGoal = nullptr;
	
	UPROPERTY(EditAnywhere, Category="Pickup")
	float PickupTimer = 2.0f;
	UPROPERTY(EditAnywhere, Category="Pickup")
	struct FBlackboardKeySelector EnemyHasPickupKey;
	UPROPERTY(EditAnywhere, Category="Pickup")
	struct FBlackboardKeySelector PlayerGoalKey;
};
