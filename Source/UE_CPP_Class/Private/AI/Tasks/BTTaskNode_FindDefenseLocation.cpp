#include "AI/Tasks/BTTaskNode_FindDefenseLocation.h"

EBTNodeResult::Type UBTTaskNode_FindDefenseLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type();
}

void UBTTaskNode_FindDefenseLocation::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
}

FString UBTTaskNode_FindDefenseLocation::GetStaticDescription() const
{
	return Super::GetStaticDescription();
}
