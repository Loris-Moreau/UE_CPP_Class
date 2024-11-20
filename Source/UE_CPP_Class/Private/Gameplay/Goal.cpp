#include "Gameplay/Goal.h"

#include <string>

#include "Engine/World.h"
#include "Gameplay/PickupComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AGoal::AGoal(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, "CollisionBox");
	if (CollisionBox)
	{
		SetRootComponent(CollisionBox);
	}
}

void AGoal::BeginPlay()
{
	Super::BeginPlay();
	// Bind to overlap
	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AGoal::OnOverlapBegin);
	}
}

void AGoal::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UPickupComponent* enteringPickup = OtherActor->FindComponentByClass<UPickupComponent>();
	
	if (!enteringPickup)
		return;
	
	score++;
	
	FString GoalName = UKismetSystemLibrary::GetDisplayName(this);
	//OnSendScore.Broadcast(score, GoalName);
}

unsigned int AGoal::CountPickupInside()
{
	if (!CollisionBox)
		return 0;
	
	// Prepare Vars
	const FVector GoalLoc = this->GetActorLocation();
	const FVector GoalExtent = CollisionBox->GetScaledBoxExtent();
	const FRotator GoalOrientation = GetActorRotation();
	const TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> HitResults;
	
	UKismetSystemLibrary::BoxTraceMulti(GetWorld(), GoalLoc, GoalLoc, GoalExtent, GoalOrientation, GoalCollisionTraceChannel, false, ActorsToIgnore, EDrawDebugTrace::None,HitResults, true);

	return HitResults.Num();
}

FString AGoal::getScore() const
{
	FString GoalName = UKismetSystemLibrary::GetDisplayName(this);
 	
 	FString scoreString = FString::FromInt(score);
	
	UE_LOG(LogTemp, Log, TEXT("Goal is %s Score is %s"), *GoalName, *scoreString);
	
 	return scoreString;
}
