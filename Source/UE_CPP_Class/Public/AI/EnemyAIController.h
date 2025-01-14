#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UCLASS(Abstract)
class UE_CPP_CLASS_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	AEnemyAIController();
	virtual void OnPossess(APawn* InPawn) override;
	
// Component
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Controller | Behaviour Tree")
	class UBehaviorTree* EnemyBehaviourTree = nullptr;
	
// End of Component
};
