#include "AI/Enemy.h"
#include "Gameplay/Goal.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
}

AGoal* AEnemy::GetPlayerGoal() const
{
	return PlayerGoal;
}

AGoal* AEnemy::GetEnemyGoal() const
{
	return EnemyGoal;
}
