#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main_Player.generated.h"

UCLASS()
class UE_CPP_CLASS_API AMain_Player : public ACharacter
{
	GENERATED_BODY()

public:
	AMain_Player();

protected:
	virtual void BeginPlay() override;
};
