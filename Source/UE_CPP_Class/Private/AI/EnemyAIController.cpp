#include "AI/EnemyAIController.h"

#include "Kismet/GameplayStatics.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Player/Main_Player.h"
#include "Gameplay/GravityGunComponent.h"


AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = false;
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
}

void AEnemyAIController::OnPlayerHasPickup(bool bInPlayerHasPickUp)
{
	// Update Blackboard Values
	if(Blackboard)
	{
		Blackboard->SetValueAsBool(PlayerHasPickupName, bInPlayerHasPickUp);
	}
}
