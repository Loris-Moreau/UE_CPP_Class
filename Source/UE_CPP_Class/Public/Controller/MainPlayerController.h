#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class APawn;
/**
 
*/
UCLASS()
class UE_CPP_CLASS_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;
	virtual void SetPawn(APawn* InPawn) override;
	
protected:
// Character
	class AMain_Player* Character = nullptr;
	
// Inputs
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Movement")
	class UInputAction* InputActionMove;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Look")
	class UInputAction* InputActionLook;

	
	void MovePlayer(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);


	// Mouse sensitivity
	UPROPERTY(EditDefaultsOnly,Category = "Sensitivity", meta=(ToolTip="Mouse Sensitivity for X axis", ClampMin="0.1",ClampMax="3.0"))
	float mouseSensiX = 1.f;
	UPROPERTY(EditDefaultsOnly,Category = "Sensitivity", meta=(ToolTip="Mouse Sensitivity for Y axis", ClampMin="0.1",ClampMax="3.0"))
	float mouseSensiY = 1.f;
	
public:
	virtual void AddPitchInput(float Val) override;
	virtual void AddYawInput(float Val) override;
};
