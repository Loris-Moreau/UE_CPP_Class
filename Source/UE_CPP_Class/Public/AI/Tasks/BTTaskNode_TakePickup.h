#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Gameplay/GravityGunComponent.h"
#include "BTTaskNode_TakePickup.generated.h"

UCLASS()
class UE_CPP_CLASS_API UBTTaskNode_TakePickup : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;

protected:
	EBTNodeResult::Type Failed();
	
	TObjectPtr<class AEnemyAIController> EnemyController;
	TObjectPtr<class AEnemy> EnemyCharacter;
	
	float RandowAngleYaw = 0.0f;
	float RandowAnglePitch = 0.0f;
	
	AActor* Pickup = nullptr;
	
	class UGravityGunComponent* GravityGunComp = nullptr;
	
	UPROPERTY(EditAnywhere, Category="Pickup")
	float RandomAngleAroundRaycast = 7.0f;
	UPROPERTY(EditAnywhere, Category="Pickup")
	float PickupTimer = 2.0f;
	UPROPERTY(EditAnywhere, Category="Pickup")
	struct FBlackboardKeySelector CanTakePickupKey;
	UPROPERTY(EditAnywhere, Category="Pickup")
	struct FBlackboardKeySelector PickupKey;
	
	UPROPERTY(EditAnywhere, Category="Pickup")
	struct FBlackboardKeySelector EnemyHasPickupKey;
};
