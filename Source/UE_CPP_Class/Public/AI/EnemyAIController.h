#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIEnums.h"
#include "EnemyAIController.generated.h"

UCLASS(Abstract)
class UE_CPP_CLASS_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	AEnemyAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	
// Component
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Controller | Behaviour Tree")
	class UBehaviorTree* EnemyBehaviourTree = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Controller | Behaviour Tree | Blackboard")
	FName PlayerHasPickupName = "PlayerHasPickup";

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Controller | Behaviour Tree | Blackboard")
	FName EnemyIsInDefenseSphereName = "EnemyIsInDefenseSphere";
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Controller | Behaviour Tree | Blackboard")
	FName EnemyIsInAttackSphereName = "EnemyIsInAttackSphere";

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Controller | Behaviour Tree | Blackboard")
	FName AttackGoalName = "AttackGoal";
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Controller | Behaviour Tree | Blackboard")
	FName DefenseGoalName = "DefenseGoal";

protected:
	UFUNCTION()
	void OnPlayerHasPickup(bool bInPlayerHasPickup);


	UFUNCTION()
	void OnActorOverlapAISphere( bool bIsOverlaped, EAIBehaviourType BehaviourType, AActor* OverlapedActor);
// End of Component
};
