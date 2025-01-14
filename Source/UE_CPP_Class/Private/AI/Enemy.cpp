#include "AI/Enemy.h"

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
