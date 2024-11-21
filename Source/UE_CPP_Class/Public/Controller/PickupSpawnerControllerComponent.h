// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickupSpawnerControllerComponent.generated.h"


UCLASS(Blueprintable, Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_CPP_CLASS_API UPickupSpawnerControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPickupSpawnerControllerComponent();
	void SetupInputComponentSpawner(TObjectPtr<class UInputComponent> InputComponent, class AMain_Player* InCharacter);
	
protected:
	virtual void BeginPlay() override;

	TWeakObjectPtr<class AMain_Player> Character = nullptr;
	TWeakObjectPtr<class APlayerCameraManager> CameraManager = nullptr;
	TWeakObjectPtr<class UPickupSpawnerComponent> SpawnerComponent = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Pick Up", meta = (ClampMin = "100.0", ClampMax = "1000.0"))
	float PickUpDistanceFromPlayer = 200.f;
	UPROPERTY(EditDefaultsOnly, Category = "Pick Up", meta = (ClampMin = "-200.0", ClampMax = "200.0"))
	float PickUpHeightFromPlayer = -10.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputAction* InputActionGetPickupAmount;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputAction* InputActionSpawnNormal;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputAction* InputActionSpawnThrowDestroy;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputAction* InputActionSpawnTakeDestroy;
	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// Pickup
	UPROPERTY(EditAnywhere, Category="Pickup")
	TSubclassOf<class UPickUpComponent> PickupNormal = nullptr;

	void GetPickupAmount();
	
	void SpawnNormal();
	void SpawnThrowDestroy();
	void SpawnTakeDestroy();

protected:
	FVector spawnPos = { -500, -500, 500};
};


// TODO :
/*
 *Done : Spawn Input
 *Done : input to let us know how many pickups there are on the map
 *Done : Pickup Quota
 *Done : Timer (min Time between 2 spawns)
 *Done : Destroy the one in hand
 **spawn pickup will be placed in hand if no pickup in hand
*/
