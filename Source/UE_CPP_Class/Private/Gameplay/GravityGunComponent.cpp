// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GravityGunComponent.h"
#include "Player/Main_Player.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Gameplay/PickUpComponent.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Gameplay/PickupSpawnerComponent.h"

#include "Curves/CurveFloat.h"

UGravityGunComponent::UGravityGunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UGravityGunComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AMain_Player>(GetOwner());
	CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	GravityGunCollisionChannel = UEngineTypes::ConvertToCollisionChannel(GravityGunCollisionTraceChannel);

	pickupSpawnerComponent = Character->GetComponentByClass<UPickupSpawnerComponent>();
}


void UGravityGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdatePickUpLocation();
	UpdateThrowForceTimer(DeltaTime);
}

void UGravityGunComponent::OnTakeObjectInputPressed()
{
	// Make sure that we don't already have a pickup in our hands
	if (CurrentPickUp.IsValid())
	{
		ReleasePickUp();
		return;
	}

	// First - Launch a raycast to find a pickup
	const FVector RaycastStart = CameraManager->GetCameraLocation();
	const FVector RaycastEnd = RaycastStart + (CameraManager->GetActorForwardVector() * RaycastSize);

	// Second - Prepare raycast params
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character.Get());

	// Third - Launch the raycast
#if !UE_BUILD_SHIPPING
	if(bDrawDebugRaycast)
		DrawDebugLine(GetWorld(), RaycastStart, RaycastEnd, FColor::Red, false, TimerDebugRaycast, 0, 3.5f);
#endif

	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, RaycastStart, RaycastEnd, GravityGunCollisionChannel, Params);
	if (!bHit)
		return;
	

	// Check for PickUp Component
	CurrentPickUpComponent = HitResult.GetActor()->GetComponentByClass<UPickUpComponent>();
	if (!CurrentPickUpComponent.IsValid())
		return;

	// Check for Mesh component
	CurrentPickUpStaticMesh = HitResult.GetActor()->GetComponentByClass<UStaticMeshComponent>();
	if (!CurrentPickUpStaticMesh.IsValid())
		return;

	// Get its pointer
	CurrentPickUp = HitResult.GetActor();
	
	// Disable the physics
	CurrentPickUpStaticMesh->SetSimulatePhysics(false);

	// Update the collision profile
	PreviousCollisionProfileName = CurrentPickUpStaticMesh->GetCollisionProfileName();
	CurrentPickUpStaticMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	// Check if destruction is required
	switch (CurrentPickUpComponent->GetPickUpType())
	{
		case EPickUpType::DestroyAfterPickUp:
			// Launch the timer
			CurrentPickUpComponent->StartPickUpDestructionTimer();

			// Bind our callback on the event
			CurrentPickUpComponent->OnPickUpDestroyed.AddUniqueDynamic(this, &UGravityGunComponent::OnHoldPickUpDestroyed);
			break;

		case EPickUpType::DestroyAfterThrow:
			// Clear the timer, so it doesn't disappear in our hands
			CurrentPickUpComponent->ClearDestructionTimer();
			break;

		default:
			break;
	}

	PickupTaken++;
	onPickupTaken.Broadcast(PickupTaken);
}

void UGravityGunComponent::OnThrowObjectInputPressed()
{
	if (!CurrentPickUp.IsValid())
		return;

	// Exercice 2 - Prepare throw force timer
	CurrentTimeToReachMaxThrowForce = 0.f;
	bUpdateThrowForceTimer = true;
	
	FPredictProjectilePathParams predictParams;
	/*
	FVector StartLocation;
	FVector LaunchVelocity;
	bool bTraceWithCollision;
	float ProjectileRadius;
	float MaxSimTime;
	bool bTraceWithChannel;
	TEnumAsByte<ECollisionChannel> TraceChannel;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<TObjectPtr<AActor>> ActorsToIgnore;
	float SimFrequency;
	float OverrideGravityZ;
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType;
	float DrawDebugTime;
	bool bTraceComplex;
	*/
	FPredictProjectilePathResult predictresults;
	UGameplayStatics::PredictProjectilePath(GetWorld(), predictParams, predictresults);
	DrawDebugSphere(GetWorld(), predictresults.LastTraceDestination.Location, 10.0f, 4, FColor::Red);
/*
	const UWorld* InWorld, 
	__resharper_unknown_type const& Center, 
	float Radius, 
	int32 Segments, 
	FColor const& Color, 
	bool bPersistentLines = false, 
	float LifeTime = -1, 
	uint8 DepthPriority = 0, 
	float Thickness = 0)
*/
}

void UGravityGunComponent::OnThrowObjectInputReleased()
{
	if (!CurrentPickUp.IsValid())
		return;

	ReleasePickUp(true);

	// Exercice 2 - Reset throw force timer
	bUpdateThrowForceTimer = false;
	CurrentTimeToReachMaxThrowForce = 0.f;
}

void UGravityGunComponent::UpdatePickUpLocation()
{
	// Make sure we have something to update
	if (!CurrentPickUp.IsValid())
		return;

	// Compute its new location
	const FVector NewPickUpLocation = CameraManager->GetCameraLocation() + (CameraManager->GetActorForwardVector() * PickUpDistanceFromPlayer) + (CameraManager->GetActorUpVector() * PickUpHeightFromPlayer);
	const FRotator NewPickUpRotation = CameraManager->GetCameraRotation();
	CurrentPickUp->SetActorLocationAndRotation(NewPickUpLocation, NewPickUpRotation);
}

void UGravityGunComponent::ReleasePickUp(bool bThrowPickUp)
{
	// Unbind event if necessary
	if (CurrentPickUpComponent->GetPickUpType() == EPickUpType::DestroyAfterPickUp)
	{
		CurrentPickUpComponent->OnPickUpDestroyed.RemoveDynamic(this, &UGravityGunComponent::OnHoldPickUpDestroyed);
	}

	// Set back the physic
	CurrentPickUpStaticMesh->SetSimulatePhysics(true);

	// Set back the coll profile
	CurrentPickUpStaticMesh->SetCollisionProfileName(PreviousCollisionProfileName);

	// Throw Pick Up
	if (bThrowPickUp)
	{
		
		float ThrowForce;
		float ThrowForceAlpha;
		
		if(dataAsset)
		{
			/*
			PickUpAngularForce = dataAsset->PickUpAngularForce;
			PickUpThrowForce = dataAsset->PickUpThrowForce;
			PickUpMaxThrowForce = dataAsset->PickUpMaxThrowForce;
			TimeToReachMaxThrowForce = dataAsset->TimeToReachMaxThrowForce;
			*/
			ThrowForceAlpha = FMath::Clamp(CurrentTimeToReachMaxThrowForce / dataAsset->TimeToReachMaxThrowForce, 0.f, 1.f);
			ThrowForce = FMath::Lerp(dataAsset->PickUpThrowForce, dataAsset->PickUpMaxThrowForce, ThrowForceAlpha) * CurrentPickUpThrowForceMultiplier;
		}
		if (CurveFloat)
		{
			ThrowForce = CurveFloat->GetFloatValue(CurrentTimeToReachMaxThrowForce) * CurrentPickUpThrowForceMultiplier;
		}
		else
		{
			ThrowForceAlpha = FMath::Clamp(CurrentTimeToReachMaxThrowForce / TimeToReachMaxThrowForce, 0.f, 1.f);
			ThrowForce = FMath::Lerp(PickUpThrowForce, PickUpMaxThrowForce, ThrowForceAlpha) * CurrentPickUpThrowForceMultiplier;
		}
		
		const FVector Impusle = CameraManager->GetActorForwardVector() * ThrowForce;
		CurrentPickUpStaticMesh->AddImpulse(Impusle);

		FVector AngularImpulse;
		if(dataAsset)
		{
			AngularImpulse = FVector(FMath::RandRange(.0, dataAsset->PickUpAngularForce.X), FMath::RandRange(.0, dataAsset->PickUpAngularForce.Y), FMath::RandRange(.0, dataAsset->PickUpAngularForce.Z));
		}
		else
		{
			AngularImpulse = FVector(FMath::RandRange(.0, PickUpAngularForce.X), FMath::RandRange(.0, PickUpAngularForce.Y), FMath::RandRange(.0, PickUpAngularForce.Z));
		}
		CurrentPickUpStaticMesh->AddAngularImpulseInDegrees(AngularImpulse);
	
		// Check if destruction is required
		if (CurrentPickUpComponent->GetPickUpType() == EPickUpType::DestroyAfterThrow)
			CurrentPickUpComponent->StartPickUpDestructionTimer();
	}

	// Clean pointers
	CurrentPickUp = nullptr;
	CurrentPickUpComponent = nullptr;
	CurrentPickUpStaticMesh = nullptr;
}

void UGravityGunComponent::OnHoldPickUpDestroyed()
{
	ReleasePickUp();
}

void UGravityGunComponent::OnIncreaseRaycastSize()
{
	RaycastSize = FMath::Clamp(RaycastSize + RaycastSizeUpdate, RaycastSizeMin, RaycastSizeMax);
	UE_LOG(LogTemp, Log, TEXT("New Racast Size is %f"), RaycastSize);
}

void UGravityGunComponent::OnDecreaseRaycastSize()
{
	RaycastSize = FMath::Clamp(RaycastSize - RaycastSizeUpdate, RaycastSizeMin, RaycastSizeMax);
	UE_LOG(LogTemp, Log, TEXT("New Racast Size is %f"), RaycastSize);
}

void UGravityGunComponent::UpdateThrowForceTimer(float DeltaTime)
{
	if (!bUpdateThrowForceTimer)
		return;

	CurrentTimeToReachMaxThrowForce += DeltaTime;
}

void UGravityGunComponent::OnThrowForceMultiplierInputPressed()
{
	CurrentPickUpThrowForceMultiplier = CurrentPickUpThrowForceMultiplier == 1.f ? PickUpThrowForceMultiplier : 1.f;
}

float UGravityGunComponent::GetTimeToReachMaxThrow()
{
	if(dataAsset)
	{
		return dataAsset->TimeToReachMaxThrowForce;
	}
	else
	{
		return TimeToReachMaxThrowForce;
	}
}

float UGravityGunComponent::GetCurrentTimeToReachMaxThrow()
{
	return CurrentTimeToReachMaxThrowForce;
}

void UGravityGunComponent::DestroyPickup(AActor* actor)
{
	if (!pickupSpawnerComponent.IsValid())
		return;

	if (actor)
	{
		pickupSpawnerComponent->destroyPickup(Cast<AActor>(actor));
	}
	else
	{
		pickupSpawnerComponent->destroyPickup(Cast<AActor>(CurrentPickUp));
	}
	
}

void UGravityGunComponent::OnUpdateGravGunDataAsstet()
{
	if (!dataAsset)
		return;

	TimeToReachMaxThrowForce = dataAsset->TimeToReachMaxThrowForce;
}

#if WITH_EDITOR
void UGravityGunComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// get what was changed
	FName propertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.GetPropertyName() : NAME_None;

	// only update the blueprint
	if (!IsTemplate() && !HasAnyFlags(RF_NeedLoad))
	{
		if (propertyName == GET_MEMBER_NAME_CHECKED(UGravityGunComponent, dataAsset))
		{
			OnUpdateGravGunDataAsstet();
		}
	}
}
#endif
