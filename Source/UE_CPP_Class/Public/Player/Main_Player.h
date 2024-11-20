#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gameplay/Goal.h"
#include "Main_Player.generated.h"

UCLASS()
class UE_CPP_CLASS_API AMain_Player : public ACharacter
{
	GENERATED_BODY()

public:
	AMain_Player();

protected:
	virtual void BeginPlay() override;
	
public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};
