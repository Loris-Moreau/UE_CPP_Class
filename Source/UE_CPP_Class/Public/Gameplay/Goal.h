// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIEnums.h"
#include "Components/PointLightComponent.h"
#include "Controller/MainPlayerController.h"
#include "GameFramework/Actor.h"
#include "Goal.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSendScoreDelegate, unsigned int, Score, FString, GoalName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAISphereOverlapDelegate, bool, bIsOverlaped,
	EAIBehaviourType, BehaviourType, AActor*, OverlapedActor);

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

	UPROPERTY(EditDefaultsOnly)
	class UPointLightComponent* pointLight = nullptr;

#if  WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
	
protected:
	void UpdatePointLight();

// AI Sphere
protected:
	UPROPERTY(EditAnywhere, Category="Goal | AI")
	EAIBehaviourType BehaviourType = EAIBehaviourType::None;	
	UPROPERTY(EditAnywhere)
	class USphereComponent* AIBehaviourCollisionSphere = nullptr;

public:
	FAISphereOverlapDelegate OnAISphereOverlap;
	
protected:
	UFUNCTION()
	void OnAICollisionSphereBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnAICollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
};
