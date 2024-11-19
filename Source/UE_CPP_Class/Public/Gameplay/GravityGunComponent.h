#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityGunComponent.generated.h"


UCLASS(Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_CPP_CLASS_API UGravityGunComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGravityGunComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void onTakeObjectInputPressed();
	void onThrowObjectInputPressed();
	void onThrowObjectInputRelease();

	void ThrowPowerCount();

	void RaySizeChange();

	float throwTimeElapsed = 0.0;
	
protected:
	TWeakObjectPtr<class AMain_Player> Character = nullptr;
	TWeakObjectPtr<class APlayerCameraManager> PlayerCameraManager = nullptr;
	
	// Collision
	UPROPERTY(EditDefaultsOnly, Category="Gravity Gun|Collision")
	TEnumAsByte<ETraceTypeQuery> GravityGunCollisionTraceChannel;
	ECollisionChannel GravityGunCollisionChannel;
	
	//UPROPERTY(EditDefaultsOnly, Category = "Gravity Gun|Collision", meta = (ClampMin = "100.0",ClampMax = "1000.0"))
	float raySize = 500.0; // 500 = 5 Meters
	UPROPERTY(EditDefaultsOnly, Category = "Gravity Gun|Collision")
	float raySizeMax = 1000.0;
	UPROPERTY(EditDefaultsOnly, Category = "Gravity Gun|Collision")
	float raySizeMin = 100.0;
	UPROPERTY(EditDefaultsOnly, Category = "Gravity Gun|Collision")
	float raySizeUpdate = 25.0;
	bool goingUp = true;
	
	// PickUp
	TWeakObjectPtr<AActor> CurrentPickup = nullptr;
	TWeakObjectPtr<class UPickupComponent> CurrentPickupComponent = nullptr;
	TWeakObjectPtr<class UStaticMeshComponent> CurrentPickupStaticMesh = nullptr;
	FName previousCollisionProfileName = NAME_None;
	
	UPROPERTY(EditAnywhere, Category = "Gravity Gun|Pickup", meta = (ClampMin = "100.0", ClampMax = "500.0"))
	float pickupDisanceFromPlayer = 200.0;
	UPROPERTY(EditAnywhere, Category = "Gravity Gun|Pickup", meta = (ClampMin = "-200.0", ClampMax = "200.0"))
	float pickupHeightFromPlayer = -15.0;
	UPROPERTY(EditAnywhere, Category = "Gravity Gun|Pickup", meta = (ClampMin = "1000.0", ClampMax = "100000.0"))
	float PickupThrowForce = 5000.0;
	UPROPERTY(EditAnywhere, Category = "Gravity Gun|Pickup")
	FVector PickupAngularForce = {450000.0, 250000.0, 500000.0};

	void UpdatePickupLocation();
	void ReleasePickup(bool throwPickup = false);
	
	// Debug
	UPROPERTY(EditAnywhere, Category="Gravity Gun|Debug")
	bool DrawDebugRaycast = false;
	UPROPERTY(EditAnywhere, Category="Gravity Gun|Debug", meta = (EditCondition = "DrawDebugRaycast", EditConditionHides,ClampMin = "0.1", ClampMax = "5.0"))
	float TimerDebugRaycast = 0.5f;
};
