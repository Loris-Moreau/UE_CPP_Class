#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS(Abstract)
class UE_CPP_CLASS_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

// Goal
protected:
	UPROPERTY(EditInstanceOnly, Category="Enemy |Goal")
	class AGoal* PlayerGoal = nullptr;
	UPROPERTY(EditInstanceOnly, Category="Enemy |Goal")
	class AGoal* EnemyGoal = nullptr;


public:
	class AGoal* GetPlayerGoal() const;
	class AGoal* GetEnemyGoal() const;
	
// end of Goal
	
};
