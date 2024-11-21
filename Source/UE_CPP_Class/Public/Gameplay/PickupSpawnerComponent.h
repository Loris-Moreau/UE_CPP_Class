// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpComponent.h"
#include "Components/ActorComponent.h"
#include "PickupSpawnerComponent.generated.h"


UCLASS(Blueprintable, Abstract,  ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_CPP_CLASS_API UPickupSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPickupSpawnerComponent();

	TArray<AActor*> pickupArray;
	
	UPROPERTY(EditDefaultsOnly, Category = "Parameters")
	TSubclassOf<AActor> PickupClassNormal;
	UPROPERTY(EditDefaultsOnly, Category = "Parameters")
	TSubclassOf<AActor> PickupClassTakeDestroy;
	UPROPERTY(EditDefaultsOnly, Category = "Parameters")
	TSubclassOf<AActor> PickupClassThrowDestroy;
	
protected:
	virtual void BeginPlay() override;
	
	AActor* spawnedActor = nullptr;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GetPickupAmount();

	void addPickuptoArray(AActor* actor);

	// 1 = normal, 2 = throw Destroy, 3 = Take Destroy
	void spawnPickup(int type, FVector pos);

	UPROPERTY(EditDefaultsOnly, Category="Parameters|Spawning", meta=(ClampMin = "1", ClampMax = "10000"))
	int maxSpawnAmount = 500;

	void destroyPickup(AActor* pickup);

	// Cooldown Timer
	UPROPERTY(EditDefaultsOnly, Category = "Parameters|Cooldown", meta = (ClampMin = "0.05", ClampMax = "10.0"))
	float CooldownTime = 1.f;
	float CurrentCooldownTime = 0.f;
	
	bool bUpdateCooldownTimer = false;
	
	void UpdateCooldownTimer(float delta);
};
