#include "Gameplay/GravityGunComponent.h"

// Sets default values for this component's properties
UGravityGunComponent::UGravityGunComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGravityGunComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UGravityGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UGravityGunComponent::onTakeObjectInputPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Take Press"));
}

void UGravityGunComponent::onThrowObjectInputTPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Throw Press"));
}

void UGravityGunComponent::onThrowObjectInputTRelease()
{
	UE_LOG(LogTemp, Log, TEXT("Throw Release"));
}

