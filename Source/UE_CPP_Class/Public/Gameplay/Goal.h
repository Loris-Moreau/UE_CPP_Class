// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Goal.generated.h"

//DECLAREDELEGAT

UCLASS()
class UE_CPP_CLASS_API AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoal(const FObjectInitializer& ObjectInitializer);
	
	unsigned int CountPickupInside();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Collision Box
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* CollisionBox = nullptr;

	int score = 0;

	UPROPERTY(EditDefaultsOnly, Category="Goal")
	TEnumAsByte<ETraceTypeQuery> GoalCollisionTraceChannel;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	FString getScore() const;

public:
	
};
