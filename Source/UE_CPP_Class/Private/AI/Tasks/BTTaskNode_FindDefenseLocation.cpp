#include "AI/Tasks/BTTaskNode_FindDefenseLocation.h"

#include "AI/Enemy.h"
#include "AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTaskNode_FindDefenseLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	if(!BlackboardComponent) return EBTNodeResult::Failed;

	// Get the Enemy Goal
	EnemyGoal = Cast<AActor>(BlackboardComponent->GetValueAsObject(EnemyGoalKey.SelectedKeyName));
	if(!EnemyGoal) return EBTNodeResult::Failed;

	// Get dist from Goal
	DefenseDistFromGoal = EnemyController->GetMaxDistanceFromGoal();

	// Try to find a Defense location
	if(!FindDefenseLocation(DefenseDistFromGoal, DefenseLocation)) return EBTNodeResult::Failed;

	// Set the Defense Location in the Blackboard
	BlackboardComponent->SetValueAsVector(DefenseLocationGoalKey.SelectedKeyName, DefenseLocation);

	return EBTNodeResult::Succeeded;
}

void UBTTaskNode_FindDefenseLocation::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Values.Add(FString::Printf(TEXT("Defense Loc Found : it's %s"), *DefenseLocation.ToString()));
	
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
}

FString UBTTaskNode_FindDefenseLocation::GetStaticDescription() const
{
	FString Description("invalid");
	// Check if the keys given by the user are valid
	if(DefenseLocationGoalKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass() &&
		EnemyGoalKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		Description = "Goal : " + EnemyGoalKey.SelectedKeyName.ToString()
		+ " - Defense Location : " + DefenseLocationGoalKey.SelectedKeyName.ToString();
	}
	
	return FString::Printf(TEXT("%s : %s"), *Super::GetStaticDescription(), *Description);
}

bool UBTTaskNode_FindDefenseLocation::FindDefenseLocation(float InMaxDistFromGoal,
	FVector& OutDefenceLocation)
{
	// Get player
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if(!PlayerCharacter) return false;

	// Get each Actor Location
	const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	const FVector GoalLocation = EnemyGoal->GetActorLocation();

	// Prepare our Vectors
	const FVector GoalToPlayer = PlayerLocation - GoalLocation;
	FVector GoalToPlayerNormalized = GoalToPlayer;
	GoalToPlayerNormalized.Normalize();

	// Try to place the Enemy in the middle
	OutDefenceLocation = GoalLocation + (GoalToPlayerNormalized * (GoalToPlayer.Size() / 2.0f));

	// If the Enemy is Above InMaxDistanceFromGoal, we'll stick to this distance
	if(FVector::Distance(GoalLocation, OutDefenceLocation) > InMaxDistFromGoal)
	{
		OutDefenceLocation = GoalLocation + (GoalToPlayerNormalized * InMaxDistFromGoal);
	}
	
	return true;
}
