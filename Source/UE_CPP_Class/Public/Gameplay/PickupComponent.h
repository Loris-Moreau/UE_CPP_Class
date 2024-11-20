// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickUpComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPickUpDestroyedDelegate);

UENUM()
enum class EPickUpType : uint8
{
	Normal,
	DestroyAfterPickUp,
	DestroyAfterThrow,
	MAX UMETA(Hidden),
};

USTRUCT(BlueprintType)
struct FPickUpStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "PickUp")
	EPickUpType PickUpType = EPickUpType::Normal;
	UPROPERTY(EditAnywhere, Category = "PickUp", meta = (EditCondition = "PickUpType != EPickUpType::Normal", EditConditionHides))
	float DestructionTimer = 5.f;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_CPP_CLASS_API UPickUpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPickUpComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(EditAnywhere, Category = "PickUp")
	FPickUpStruct PickUpStruct;


	// Destruction
protected:
	FTimerHandle PickUpDestructionTimerHandle;

public:
	FOnPickUpDestroyedDelegate OnPickUpDestroyed;

protected:
	void DestroyPickUp();

public:
	void ClearDestructionTimer();
	void StartPickUpDestructionTimer();
	EPickUpType GetPickUpType() const;

	// End of Destruction
};
