#include "AI/EnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Gameplay/GravityGunComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Main_Player.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Start the behaviour tree
	if (EnemyBehaviourTree)
	{
		RunBehaviorTree(EnemyBehaviourTree);
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	// Get Gravity Gun Component from the player & bind to event
	if(AMain_Player* MainPlayer =
		Cast<AMain_Player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		if(UGravityGunComponent* GravityGunComponent = MainPlayer->FindComponentByClass<UGravityGunComponent>())
		{
			GravityGunComponent->OnPlayerHasPickup.AddUniqueDynamic(this, &AEnemyAIController::OnPlayerHasPickup);
		}
	}
}

void AEnemyAIController::OnPlayerHasPickup(bool bInPlayerHasPickup)
{
	// Update Blackboard Values
	if(Blackboard)
	{
		Blackboard->SetValueAsBool(PlayerHasPickupName, bInPlayerHasPickup);
	}
}
