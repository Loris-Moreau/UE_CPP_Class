// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/MainPlayerController.h"
#include "GameFramework/Actor.h"
#include "Goal.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSendScoreDelegate, unsigned int, Score, FString, GoalName);

UCLASS()
class UE_CPP_CLASS_API AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	AGoal(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	// Collision Box
protected:
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* CollisionBox = nullptr;
	unsigned int Score = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Goal")
	TEnumAsByte<ETraceTypeQuery> GoalCollisionTraceChannel;
	
protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	unsigned int CountPickUpInside();

	// End of Collision Box


	// Exercice 3
public:
	FSendScoreDelegate OnSendScore;
	
	void DisplayScore() const;

	// End of Exercice 3

	//Ex 4

	
	//End of Ex 4
};
