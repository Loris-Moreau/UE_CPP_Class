#include "AI/Tasks/BTTask_UpdateSpeed.h"
#include "AI/Enemy.h"
#include "AI/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTTask_UpdateSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the Character & Controller
	if(EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		EnemyCharacter = Cast<AEnemy>(EnemyController->GetPawn());
	}

	// Check if we've got the character
	if(!EnemyCharacter) return EBTNodeResult::Failed;

	// Get the Speed
	CharacterSpeed = EnemyController->GetSpeedType(SpeedType);

	// Apply the speed
	EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = CharacterSpeed;

	return EBTNodeResult::Succeeded;
}

void UBTTask_UpdateSpeed::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Values.Add(FString::Printf(TEXT("Chara Speed is %f"), CharacterSpeed));
	
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
	
}

FString UBTTask_UpdateSpeed::GetStaticDescription() const
{
	FString EnumString;
	UEnum::GetValueAsString<EAISpeedType>(SpeedType, EnumString);

	return FString::Printf(TEXT("%s : %s"), * Super::GetStaticDescription(), *EnumString);
}
