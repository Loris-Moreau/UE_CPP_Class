#pragma once

#include "CoreMinimal.h"
#include "AIEnums.generated.h"

UENUM()
enum class EAIBehaviourType : uint8
{
	None,
	Attack,
	Defense,
	MAX UMETA(Hidden),
};
