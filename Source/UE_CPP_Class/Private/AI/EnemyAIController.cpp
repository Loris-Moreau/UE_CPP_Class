#include "AI/EnemyAIController.h"

#include "AI/Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Gameplay/Goal.h"
#include "Gameplay/GravityGunComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Main_Player.h"


AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Start the Enemy Behavior Tree
	if (EnemyBehaviourTree)
	{
		RunBehaviorTree(EnemyBehaviourTree);
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	// Get the gravity gun component from the player and bind to its event
	if (AMain_Player* MainPlayer = Cast<AMain_Player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		if (UGravityGunComponent* GravityGunComponent = MainPlayer->FindComponentByClass<UGravityGunComponent>())
		{
			GravityGunComponent->OnPlayerHasPickup.AddUniqueDynamic(this, &AEnemyAIController::OnPlayerHasPickup);
		}
	}

	// Goal
	if(AEnemy* EnemyCharacter = Cast<AEnemy>(GetCharacter()))
	{
		// Get Goals
		AGoal* PlayerGoal = EnemyCharacter->GetPlayerGoal();
		AGoal* EnemyGoal = EnemyCharacter->GetEnemyGoal();
		
		if(PlayerGoal && EnemyGoal && Blackboard)
		{
			// Set Blackboard Values
			Blackboard->SetValueAsObject(AttackGoalName, PlayerGoal);
			Blackboard->SetValueAsObject(DefenseGoalName, EnemyGoal);

			// Bind Goal Overlap Event
			PlayerGoal->OnAISphereOverlap.AddUniqueDynamic(this, &AEnemyAIController::OnActorOverlapAISphere);
			EnemyGoal->OnAISphereOverlap.AddUniqueDynamic(this, &AEnemyAIController::OnActorOverlapAISphere);
		}
	}

	// Fix cause AI is inside Defense Sphere
	// do only if the AI is in defense Sphere at beginplay 
	Blackboard->SetValueAsBool(EnemyIsInDefenseSphereName, true);
}

void AEnemyAIController::OnPlayerHasPickup(bool bInPlayerHasPickUp)
{
	// Update Blackboard Values
	if(Blackboard)
	{
		Blackboard->SetValueAsBool(PlayerHasPickupName, bInPlayerHasPickUp);
	}
}

void AEnemyAIController::OnActorOverlapAISphere(bool bIsOverlaped, EAIBehaviourType BehaviourType, AActor* OverlapedActor)
{
	// Check if it's the Enemy
	AEnemy* Enemy = Cast<AEnemy>(OverlapedActor);
	if(!Enemy)
	{
		return;
	}

	// Update Blackboard Values
	if(!Blackboard)
	{
		return;
	}
	Blackboard->SetValueAsBool(BehaviourType == EAIBehaviourType::Attack ?
		EnemyIsInAttackSphereName : EnemyIsInDefenseSphereName, bIsOverlaped);
	
}

float AEnemyAIController::GetSpeedType(EAISpeedType InType) const
{
	if(!SpeedDataAsset) return 0.0f;

	return SpeedDataAsset->SpeedTypeArray[(uint8)InType];
}

float AEnemyAIController::GetMaxDistanceFromGoal() const
{
	return MaxDistanceFromGoal;
}
