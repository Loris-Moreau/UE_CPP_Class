#include "Gameplay/PickupComponent.h"

UPickupComponent::UPickupComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UPickupComponent::BeginPlay()
{
	Super::BeginPlay();
	
}
