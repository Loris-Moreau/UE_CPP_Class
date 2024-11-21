// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PickupSpawnerComponent.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/PickupComponent.h"

UPickupSpawnerComponent::UPickupSpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPickupSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();


	//Add Pickups Already on Scene, don't question the logic if it works.
	TArray<AActor*> allSceneActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allSceneActors);
	TArray<UActorComponent*> timTam;
	for (AActor* actors : allSceneActors)
	{
		actors->GetComponents(timTam);
		bool isPickupComponent = timTam.FindItemByClass<UPickUpComponent>();
		if(isPickupComponent)
		{
			pickupArray.Add(actors);
		}
	}
}

void UPickupSpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPickupSpawnerComponent::GetPickupAmount()
{
	int amountOfPickups = pickupArray.Num();
	
	UE_LOG(LogTemp, Log, TEXT("There Are %d Pickups on the map"), amountOfPickups)
	
}

void UPickupSpawnerComponent::spawnPickup(int type, FVector pos)
{
	FRotator rot = {0, 0, 0};

	FActorSpawnParameters spawnParams;

	if (pickupArray.Num() >= maxSpawnAmount)
	{
		return;
	}
	
	if (type == 1)
	{
		spawnedActor = GetWorld()->SpawnActor<AActor>(PickupClassNormal, pos, rot, spawnParams);
	}
	if (type == 2)
	{
		spawnedActor = GetWorld()->SpawnActor<AActor>(PickupClassThrowDestroy, pos, rot, spawnParams);
	}
	if (type == 3)
	{
		spawnedActor = GetWorld()->SpawnActor<AActor>(PickupClassTakeDestroy, pos, rot, spawnParams);
	}
	
	if (!spawnedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pickup Spawn Failed"));
		return;
	}
	
	pickupArray.Add(spawnedActor);
}

void UPickupSpawnerComponent::destroyPickup()
{
	int indexCheck = pickupArray.Num()-1;
	if (!pickupArray.IsValidIndex(indexCheck))
		return;
	
	AActor* actorToDestroy = pickupArray.Last();
	
	if (!actorToDestroy)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor to Destroy is invalid"))
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Actor Destroyed %s"), *actorToDestroy->GetName());
	
	pickupArray.RemoveAt(indexCheck);
	
	actorToDestroy->Destroy();
}
