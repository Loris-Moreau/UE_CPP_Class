#include "Gameplay/GravityGunComponent.h"

#include "Engine/World.h"
#include "Public/Player/Main_Player.h"
#include "Kismet/GameplayStatics.h"

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
	
	// Third - Launch Raycast
	const bool bHit =	GetWorld()->LineTraceSingleByChannel(HitResult, RaycastStart, RaycastEnd, GravityGunCollisionChannel, Params);
	if(!bHit)
		return;
	
}

void UGravityGunComponent::onThrowObjectInputPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Throw Press"));
}

void UGravityGunComponent::onThrowObjectInputRelease()
{
	UE_LOG(LogTemp, Log, TEXT("Throw Release"));
}

