#include "AI/Tasks/BTTask_ThrowPickup.h"

#include "AI/Enemy.h"
#include "AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

EBTNodeResult::Type UBTTask_ThrowPickup::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the Character & Controller
	if((EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner())))
	{
		EnemyCharacter = Cast<AEnemy>(EnemyController->GetPawn());
	}
	
	// Check if we've got the character
	if(!EnemyCharacter) return EBTNodeResult::Failed;
	
	// Get the Blackboard Component
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if(!BlackboardComponent) return EBTNodeResult::Failed();
	
	// Get the Player Goal
	PlayerGoal = Cast<AActor>(BlackboardComponent->GetValueAsObject(PlayerGoalKey.SelectedKeyName));
	if(!PlayerGoal) return EBTNodeResult::Failed;

	
	// get grav gun comp
	GravityGunComp = EnemyCharacter->FindComponentByClass<UGravityGunComponent>();
	if (!GravityGunComp) return EBTNodeResult::Failed();

	const FVector RaycastStart = EnemyCharacter->GetActorLocation();
	const FVector RaycastEnd = PlayerGoal->GetActorLocation();

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(EnemyCharacter);
	Params.AddIgnoredActor(PlayerGoal);
	FHitResult RaycastHit

	if(GetWorld()->LineTraceSingleByChannel(RaycastHit, RaycastStart, RaycastEnd, ECC_Visibility, Params))
	{
		return EBTNodeResult::Failed;
	}

	GravityGunComp->ReleasePickUp(true);
	EnemyController->StartCanTakePickupTimer(PickupTimer);

	BlackboardComponent->SetValueAsBool(EnemyHasPickupKey.SelectedKeyName, false);

	
}

void UBTTask_ThrowPickup::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
}

FString UBTTask_ThrowPickup::GetStaticDescription() const
{
	FString Description("invalid");
	// Check if the keys given by the user are valid
	if(EnemyHasPickupKey.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass() &&
		PlayerGoalKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		Description = "Enemy Has Pick Up : " + EnemyHasPickupKey.SelectedKeyName.ToString()
		+ " - Player Goal : " + PlayerGoal.SelectedKeyName.ToString()
		+ FString(TEXT(" - Pickup Timer %f"), PickupTimer);
	}
	
	return FString::Printf(TEXT("%s : %s"), *Super::GetStaticDescription(), *Description);
}
