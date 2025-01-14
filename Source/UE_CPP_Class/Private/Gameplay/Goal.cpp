// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Goal.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Gameplay/PickUpComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AGoal::AGoal(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, "CollisionBox");
	if (CollisionBox)
	{
		SetRootComponent(CollisionBox);
	}

	pointLight = ObjectInitializer.CreateDefaultSubobject<UPointLightComponent>(this, "PointLight");
	if(pointLight)
	{
		pointLight->SetupAttachment(RootComponent);
	}

	// AI Sphere
	
	AIBehaviourCollisionSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, "AIBehaviourSphere");
	if(AIBehaviourCollisionSphere)
	{
		AIBehaviourCollisionSphere->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();

	// Bind to the overlap event
	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AGoal::OnOverlapBegin);
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Goal Collision Box Missing"));
	}

	// Bind to sphere Events
	if(AIBehaviourCollisionSphere)
	{
		AIBehaviourCollisionSphere->OnComponentBeginOverlap.AddUniqueDynamic(
			this, &AGoal::OnAICollisionSphereBeginOverlap);
		AIBehaviourCollisionSphere->OnComponentEndOverlap.AddUniqueDynamic(
			this, &AGoal::OnAICollisionSphereEndOverlap);
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Goal Sphere Missing"));
	}
}

void AGoal::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UPickUpComponent* EnteringPickUp = OtherActor->FindComponentByClass<UPickUpComponent>();
	if (!EnteringPickUp)
		return;
	
	Score++;
	
	FString GoalName = UKismetSystemLibrary::GetDisplayName(this);
	OnSendScore.Broadcast(Score, GoalName);
}

unsigned int AGoal::CountPickUpInside()
{
	// Check collision box
	if (!CollisionBox)
		return 0;

	// Prepare variables
	const FVector GoalLocation = GetActorLocation();
	const FVector BoxSize = CollisionBox->GetScaledBoxExtent();
	const FRotator GoalOrientation = GetActorRotation();
	const TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> HitResults;
	
	UKismetSystemLibrary::BoxTraceMulti(GetWorld(), GoalLocation, GoalLocation, BoxSize, GoalOrientation, 
		GoalCollisionTraceChannel,false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);
	
	return HitResults.Num();
}

void AGoal::DisplayScore() const
{
	FString GoalName = UKismetSystemLibrary::GetDisplayName(this);
	UE_LOG(LogTemp, Log, TEXT("%s has a score of %d"), *GoalName, Score);
}

void AGoal::UpdatePointLight()
{
	if (!pointLight)
 		return;
	
	pointLight->SetLightColor(FLinearColor::Red);
}

void AGoal::OnAICollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnAISphereOverlap.Broadcast(true, BehaviourType, OtherActor);
}

void AGoal::OnAICollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OnAISphereOverlap.Broadcast(false, BehaviourType, OtherActor);
}

#if WITH_EDITOR
void AGoal::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if(!IsTemplate() && !HasAnyFlags(RF_NeedLoad))
	{
		UpdatePointLight();
	}
}
#endif
