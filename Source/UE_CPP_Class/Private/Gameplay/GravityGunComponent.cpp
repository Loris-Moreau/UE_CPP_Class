#include "Gameplay/GravityGunComponent.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Framework/MultiBox/MultiBoxDefs.h"
#include "Public/Player/Main_Player.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Gameplay/PickupComponent.h"

UGravityGunComponent::UGravityGunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UGravityGunComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AMain_Player>(GetOwner());
	PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	GravityGunCollisionChannel = UEngineTypes::ConvertToCollisionChannel(GravityGunCollisionTraceChannel);
}

void UGravityGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UGravityGunComponent::onTakeObjectInputPressed()
{
	// First - Launch RayCast
	FVector RaycastStart = PlayerCameraManager->GetCameraLocation();
	FVector RaycastEnd = RaycastStart + PlayerCameraManager->GetActorForwardVector() * raySize;

	// Second - Prepare Param
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character.Get());

#if !UE_BUILD_SHIPPING
	if (DrawDebugRaycast)
	{
		DrawDebugLine(GetWorld(), RaycastStart, RaycastEnd, FColor::Red, false, TimerDebugRaycast, 0, 0.5);
	}
#endif
	
	// Third - Launch Raycast
	const bool bHit =	GetWorld()->LineTraceSingleByChannel(HitResult, RaycastStart, RaycastEnd, GravityGunCollisionChannel, Params);
	if(!bHit)
		return;
	
	// Check for pickup component 
	CurrentPickupComponent = HitResult.GetActor()->GetComponentByClass<UPickupComponent>();
	if (!CurrentPickupComponent.IsValid())
		return;

	// Gets it's pointer
	CurrentPickup = HitResult.GetActor();
	
	FString ActorName = UKismetSystemLibrary::GetDisplayName(HitResult.GetActor());
	UE_LOG(LogTemp, Log, TEXT("Hit : %s"), *ActorName);

}

void UGravityGunComponent::onThrowObjectInputPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Throw Press"));
}

void UGravityGunComponent::onThrowObjectInputRelease()
{
	UE_LOG(LogTemp, Log, TEXT("Throw Release"));
}

