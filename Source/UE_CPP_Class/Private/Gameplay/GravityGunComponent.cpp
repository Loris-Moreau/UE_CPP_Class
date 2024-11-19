#include "Gameplay/GravityGunComponent.h"

#include <cmath>

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Framework/MultiBox/MultiBoxDefs.h"
#include "Public/Player/Main_Player.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Gameplay/PickupComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

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

	UpdatePickupLocation();
}

void UGravityGunComponent::onTakeObjectInputPressed()
{
	// make sure to only have one pickup in hand
	if (CurrentPickup.IsValid())
	{
		ReleasePickup();
		return;
	}
	
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

	// Check for mesh component
	CurrentPickupStaticMesh = HitResult.GetActor()->GetComponentByClass<UStaticMeshComponent>();
	if (!CurrentPickupStaticMesh.IsValid())
		return;
	
	// Gets it's pointer
	CurrentPickup = HitResult.GetActor();
	if (CurrentPickup.IsValid())
		return;
	
	//FString ActorName = UKismetSystemLibrary::GetDisplayName(HitResult.GetActor());
	//UE_LOG(LogTemp, Log, TEXT("Hit : %s"), *ActorName);

	// Update collision profile & physics
	previousCollisionProfileName = CurrentPickupStaticMesh->GetCollisionProfileName();
	CurrentPickupStaticMesh->SetSimulatePhysics(false);
	CurrentPickupStaticMesh->SetEnableGravity(false);
	CurrentPickupStaticMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void UGravityGunComponent::onThrowObjectInputPressed()
{
	if (!CurrentPickup.IsValid())
 		return;

	
	UE_LOG(LogTemp, Log, TEXT("Throw Press"));
	
}

void UGravityGunComponent::onThrowObjectInputRelease()
{
	if (!CurrentPickup.IsValid())
		return;
	
	
	ReleasePickup(true);
	
	UE_LOG(LogTemp, Log, TEXT("Throw Release"));
}

void UGravityGunComponent::ThrowPowerCount()
{
	//FMath::FInterpTo(PickupThrowForce, PickupMaxThrowForce, throwTimeElapsed, 1);
	PickupThrowForce = FMath::Lerp(PickupThrowForce, PickupMaxThrowForce, throwTimeElapsed);
	
	if (PickupThrowForce >= PickupMaxThrowForce)
	{
		PickupThrowForce = PickupMaxThrowForce;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Force : %f"), PickupThrowForce)
}

void UGravityGunComponent::RaySizeChange()
{
	if (raySize >= raySizeMax)
	{
		goingUp = false;
	}
	if (raySize <= raySizeMin)
	{
		goingUp = true;
	}
	
	if (goingUp == true)
	{
		raySize += raySizeUpdate;
	}
	else if (goingUp == false)
	{
		raySize -= raySizeUpdate;
	}
	
	//UE_LOG(LogTemp, Log, TEXT("ray size : %f"), raySize);
}

void UGravityGunComponent::UpdatePickupLocation()
{
	if (!CurrentPickup.IsValid())
		return;

	const FVector NewPickupLocation = PlayerCameraManager->GetCameraLocation() + (PlayerCameraManager->GetActorForwardVector() * pickupDisanceFromPlayer) + PlayerCameraManager->GetActorUpVector() * pickupHeightFromPlayer;
	const FRotator NewPickupRotation = PlayerCameraManager->GetCameraRotation();

	CurrentPickup->SetActorLocationAndRotation(NewPickupLocation, NewPickupRotation);

	//UE_LOG(LogTemp, Log, TEXT("pos : %s"), *NewPickupLocation.ToString());
}

void UGravityGunComponent::ReleasePickup(bool throwPickup)
{
	if (!CurrentPickup.IsValid())
		return;

	// Reset collision & Physics
	CurrentPickupStaticMesh->SetCollisionProfileName(previousCollisionProfileName);
	CurrentPickupStaticMesh->SetSimulatePhysics(true);
	CurrentPickupStaticMesh->SetEnableGravity(true);
	
	if (throwPickup)
	{
		const FVector ImpulseForce = PlayerCameraManager->GetActorForwardVector() * PickupThrowForce;
		CurrentPickupStaticMesh->AddImpulse(ImpulseForce);
		
		const FVector AngularImpulse = {FMath::RandRange(.0, PickupAngularForce.X), FMath::RandRange(.0, PickupAngularForce.Y), FMath::RandRange(.0, PickupAngularForce.Z)};
		CurrentPickupStaticMesh->AddAngularImpulseInDegrees(AngularImpulse);
		
		// check for destroy
		if (CurrentPickupComponent->GetPickupType() == EPickupType::DestroyOnThrow)
			CurrentPickupComponent->StartDestructionTimer();
	}
	
	// Resets Refs
	CurrentPickup = nullptr;
	CurrentPickupComponent = nullptr;
	CurrentPickupStaticMesh = nullptr;
}
