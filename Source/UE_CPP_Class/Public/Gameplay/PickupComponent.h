// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickupComponent.generated.h"

UENUM()
enum class EPickupType : uint8
{
	Normal,
	DestroyOnPickup,
	DestroyOnThrow,
	MAX UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FPickupStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Pickup")
	EPickupType pickupType = EPickupType::Normal;
	UPROPERTY(EditAnywhere, Category = "Pickup", meta = (EditCondition = "pickupType != EPickupType::Normal", EditConditionHides))
	float destructionTimer = 5.f;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_CPP_CLASS_API UPickupComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPickupComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, Category = "Pickup")
	FPickupStruct PickupStruct;

	// Destroy
	FTimerHandle PickupDestructionTimerHandle;
	
	void DestroyPickup();

public:
	void StartDestructionTimer();
	EPickupType GetPickupType() const;
};
