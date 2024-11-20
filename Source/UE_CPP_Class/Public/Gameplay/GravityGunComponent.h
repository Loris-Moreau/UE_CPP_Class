// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityGunComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNumPickupDelegate, int, pickupTaken);

UCLASS(Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_CPP_CLASS_API UGravityGunComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGravityGunComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void OnTakeObjectInputPressed();
	void OnThrowObjectInputPressed();
	void OnThrowObjectInputReleased();

protected:
	TWeakObjectPtr<class AMain_Player> Character = nullptr;
	TWeakObjectPtr<class APlayerCameraManager> CameraManager = nullptr;
	
// Collision
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GravityGun|Collision")
	TEnumAsByte<ETraceTypeQuery> GravityGunCollisionTraceChannel;
	ECollisionChannel GravityGunCollisionChannel;

	UPROPERTY(EditDefaultsOnly, Category = "GravityGun|Collision", meta = (ClampMin = "100.0", ClampMax = "1000.0"))
	float RaycastSize = 500.f;

// End of Collision
	
// Pick Up
protected:
	TWeakObjectPtr<AActor> CurrentPickUp = nullptr;
	TWeakObjectPtr<class UPickUpComponent> CurrentPickUpComponent = nullptr;
	TWeakObjectPtr<class UStaticMeshComponent> CurrentPickUpStaticMesh = nullptr;
	FName PreviousCollisionProfileName = NAME_None;

	UPROPERTY(EditDefaultsOnly, Category = "GravityGun|Pick Up", meta = (ClampMin = "100.0", ClampMax = "1000.0"))
	float PickUpDistanceFromPlayer = 200.f;
	UPROPERTY(EditDefaultsOnly, Category = "GravityGun|Pick Up", meta = (ClampMin = "-200.0", ClampMax = "200.0"))
	float PickUpHeightFromPlayer = -10.f;
	UPROPERTY(EditDefaultsOnly, Category = "GravityGun|Pick Up", meta = (ClampMin = "100.0", ClampMax = "10000.0"))
	float PickUpThrowForce = 5000.f;
	UPROPERTY(EditDefaultsOnly, Category = "GravityGun|Pick Up")
	FVector PickUpAngularForce = FVector(50000.f, 30000.f, 60000.f);

protected:
	void UpdatePickUpLocation();
	void ReleasePickUp(bool bThrowPickUp = false);

	UFUNCTION()
	void OnHoldPickUpDestroyed();

// End of Pick Up


// Ex 1
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GravityGun|Collision|Exercice 1", meta = (ClampMin = "100.0", ClampMax = "1000.0"))
	float RaycastSizeMin = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "GravityGun|Collision|Exercice 1", meta = (ClampMin = "100.0", ClampMax = "1000.0"))
	float RaycastSizeMax = 1000.f;
	UPROPERTY(EditDefaultsOnly, Category = "GravityGun|Collision|Exercice 1", meta = (ClampMin = "100.0", ClampMax = "1000.0", ToolTip = "Size that will be changed on the raycast each time you increase or decrease it."))
	float RaycastSizeUpdate = 100.f;


public:
	void OnIncreaseRaycastSize();
	void OnDecreaseRaycastSize();

// End of Ex 1


// Ex 2
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GravityGun|Pick Up", meta = (ClampMin = "100.0", ClampMax = "10000.0"))
	float PickUpMaxThrowForce = 10000.f;
	UPROPERTY(EditDefaultsOnly, Category = "GravityGun|Pick Up", meta = (ClampMin = "0.1", ClampMax = "10.0"))
	float TimeToReachMaxThrowForce = 3.f;
	float CurrentTimeToReachMaxThrowForce = 0.f;
	
	bool bUpdateThrowForceTimer = false;

	UPROPERTY(EditDefaultsOnly, Category = "GravityGun|Pick Up", meta = (ClampMin = "0.1", ClampMax = "10.0"))
	float PickUpThrowForceMultiplier = 3.f;
	float CurrentPickUpThrowForceMultiplier = 1.f;

protected:
	void UpdateThrowForceTimer(float DeltaTime);

public:
	void OnThrowForceMultiplierInputPressed();
	
// End of Ex 2

	// event
	int PickupTaken = 0;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FNumPickupDelegate onPickupTaken;
	// end of event

// Debug
protected:
	UPROPERTY(EditAnywhere, Category = "GravityGun|Debug")
	bool bDrawDebugRaycast = false;
	UPROPERTY(EditAnywhere, Category = "GravityGun|Debug", meta = (EditCondition = "bDrawDebugRaycast", EditConditionHides, ClampMin = "0.5", ClampMax = "10.0"))
	float TimerDebugRaycast = 5.f;

// End of Debug

	// UI
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gravity Gun")
	float GetTimeToReachMaxThrow();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Gravity Gun")
	float GetCurrentTimeToReachMaxThrow();
	
	//end of UI
};
