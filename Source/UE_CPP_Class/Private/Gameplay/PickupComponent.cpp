#include "Gameplay/PickupComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

UPickupComponent::UPickupComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UPickupComponent::BeginPlay()
{
	Super::BeginPlay();
}
