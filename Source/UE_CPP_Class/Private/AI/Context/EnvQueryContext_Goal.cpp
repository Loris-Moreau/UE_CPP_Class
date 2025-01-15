#include "AI/Context/EnvQueryContext_Goal.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Gameplay/Goal.h"
#include "Kismet/GameplayStatics.h"

void UEnvQueryContext_Goal::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	// Check Querier
	UObject* QuerierObject = QueryInstance.Owner.Get();
	if(!QuerierObject) return;
	// Check Goal BP
	if(!GoalBP) return;
	
	// Try to get the Goal with the correct behavior
	TArray<AActor*> GoalArray, GoalAIBehaviorArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), GoalBP, GoalArray);
	if(GoalArray.IsEmpty()) return;

	// Look for our Goals
	for (AActor* Goal : GoalArray)
	{
		if(AGoal* GoalCasted = Cast<AGoal>(Goal))
		{
			if (GoalCasted->GetBehaviourType() == GoalBehaviorType)
			{
				GoalAIBehaviorArray.Add(GoalCasted);
			}
		}
	}

	// Send the Goal we found
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, GoalAIBehaviorArray);
}
