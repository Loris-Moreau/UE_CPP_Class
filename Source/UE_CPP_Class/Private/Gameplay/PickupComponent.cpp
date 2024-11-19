#include "Gameplay/PickupComponent.h"

#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"

UPickupComponent::UPickupComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UPickupComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPickupComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Clear Timer
	FTimerManager& TimerManager = this->GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(PickupDestructionTimerHandle);

	// Need to call after because it cleans up
	Super::EndPlay(EndPlayReason);
}

void UPickupComponent::StartDestructionTimer()
{
	// check
	if(PickupStruct.pickupType == EPickupType::Normal)
		return;
	
	// Get Time
	const float timeToDestroy = PickupStruct.destructionTimer;
	// Timer Manager
	FTimerManager& TimerManager = this->GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(PickupDestructionTimerHandle);
	TimerManager.SetTimer(PickupDestructionTimerHandle, this, &UPickupComponent::DestroyPickup, timeToDestroy, false);
}

void UPickupComponent::DestroyPickup()
{
	// Clear Timer
	FTimerManager& TimerManager = this->GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(PickupDestructionTimerHandle);
	
	GetOwner()->Destroy();
}

EPickupType UPickupComponent::GetPickupType() const
{
	return PickupStruct.pickupType;
}
