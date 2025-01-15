#pragma once

#include "CoreMinimal.h"
#include "AIEnums.generated.h"

UENUM()
enum EAISpeedType : uint8
{
	Attack,
	Defense,
	ReachPickup,
	MAX UMETA(Hidden),
};

UENUM()
enum class EAIBehaviourType : uint8
{
	None,
	Attack,
	Defense,
	MAX UMETA(Hidden),
};
