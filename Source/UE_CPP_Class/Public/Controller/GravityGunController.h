#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityGunController.generated.h"


UCLASS(Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_CPP_CLASS_API UGravityGunController : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGravityGunController();

	void SetupInputComponentGravityGunController(TObjectPtr<class UInputComponent> InputComponent);

	
protected:
	virtual void BeginPlay() override;

	void onTakeObjectInputPressed();
	void onThrowObjectInputTriggered(const struct FInputActionValue& Value);
	
// Inputs
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Input Action")
	class UInputAction* InputActionTake;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Input Action")
	class UInputAction* InputActionThrow;		
};
