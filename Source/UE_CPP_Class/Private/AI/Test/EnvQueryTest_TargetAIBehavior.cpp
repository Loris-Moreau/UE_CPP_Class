#include "AI/Test/EnvQueryTest_TargetAIBehavior.h"

#include "AI/AITargetPoint.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"

// we need to create the constructor in the cpp
UEnvQueryTest_TargetAIBehavior::UEnvQueryTest_TargetAIBehavior(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Indicate the test cost & return Item Type
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();

	// Indicate if we're going to use float values
	SetWorkOnFloatValues(false);
}

void UEnvQueryTest_TargetAIBehavior::RunTest(FEnvQueryInstance& QueryInstance) const
{
	// Check if we've got an owwner
	UObject* QuerierOwner = QueryInstance.Owner.Get();
	if(!QuerierOwner) return;

	// we need to prepare this value, so we can use the user's info
	BoolValue.BindData(QuerierOwner, QueryInstance.QueryID);
	bool bWantsValid = BoolValue.GetValue();

	// Try to Cast each Instance as a Target Point
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		if (const AAITargetPoint* TargetPoint = Cast<AAITargetPoint>(GetItemActor(QueryInstance, It.GetIndex())))
		{
			// Check if there is the correct behavior Type
			if(TargetPoint->GetTargetBehaviorType() == TargetBehaviorType)
			{
				// correct one, pass
				It.SetScore(TestPurpose, FilterType, true, bWantsValid);
			}
			else
			{
				// incorrect one, fail
				It.ForceItemState(EEnvItemStatus::Failed);
			}
		}
		
	}
}

FText UEnvQueryTest_TargetAIBehavior::GetDescriptionTitle() const
{
	// get the current Title
	FString BehaviorString = TargetBehaviorType == EAIBehaviourType::Attack ? "Attack Behavior" : "Defense Behavior";

	switch (TargetBehaviorType)
	{
	case EAIBehaviourType::None:
		BehaviorString = "Nothing is selected";
		break;
	case EAIBehaviourType::Attack:
		BehaviorString = "Attack Behavior";
		break;
	case EAIBehaviourType::Defense:
		BehaviorString = "Defense Behavior";
		break;
	default:
		BehaviorString = "Nothing is selected";
		break;
		
	}
	
	// return the Parents Title and this one
	return FText::FromString(FString::Printf(TEXT("%s : %s"), *Super::GetDescriptionTitle().ToString(), *BehaviorString));
	
}

FText UEnvQueryTest_TargetAIBehavior::GetDescriptionDetails() const
{
	return DescribeBoolTestParams("behavior");

	
}
