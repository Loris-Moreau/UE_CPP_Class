// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

UCLASS()
class UE_CPP_CLASS_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void SetPawn(APawn* InPawn) override;

// Character
protected:
	class AMain_Player* Character = nullptr;
	TWeakObjectPtr<class UGravityGunController> GravityGunController = nullptr;
	TWeakObjectPtr<class UPickupSpawnerControllerComponent> SpawnerController = nullptr;

// Inputs
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Movement")
	class UInputAction* InputActionMove;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Look")
	class UInputAction* InputActionLook;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Movement")
	class UInputAction* InputActionJump;

protected:
	void MovePlayer(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);
	void Jump();

// End of Inputs


// Mouse Sensitivity
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Sensitivity", meta = (ToolTip = "Change the X axis sensitivity.", ClampMin = "0.1", ClampMax = "3.0"))
	float MouseSensitivityX = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Sensitivity", meta = (ToolTip = "Change the Y axis sensitivity.", ClampMin = "0.1", ClampMax = "3.0"))
	float MouseSensitivityY = 1.f;

public:
	virtual void AddPitchInput(float Value) override;
	virtual void AddYawInput(float Value) override;

// End of Mouse Sensitivity


// Exercice 3
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input|Score")
	class UInputAction* InputActionScore;

	TArray<AActor*> GoalArray;

	UFUNCTION()
	void OnSendScore(unsigned int Score, FString GoalName);

protected:
	void OnDisplayScoreInputPressed();

// End of Exercice 3
};
