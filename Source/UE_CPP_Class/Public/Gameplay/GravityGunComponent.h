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


protected:
	TWeakObjectPtr<class AMain_Player> Character = nullptr;
	TWeakObjectPtr<class APlayerCameraManager> PlayerCameraManager = nullptr;
	
	// Collision
	UPROPERTY(EditDefaultsOnly, Category="Gravity Gun|Collision")
	TEnumAsByte<ETraceTypeQuery> GravityGunCollisionTraceChannel;
	ECollisionChannel GravityGunCollisionChannel;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gravity Gun|Collision", meta = (ClampMin = "100.0",ClampMax = "1000.0"))
	float raySize = 500.0; // 500 = 5 Meters

	// PickUp
	TWeakObjectPtr<AActor> CurrentPickup = nullptr;
	TWeakObjectPtr<class UPickupComponent> CurrentPickupComponent = nullptr;
	
	// Debug
	UPROPERTY(EditAnywhere, Category="Gravity Gun|Debug")
	bool DrawDebugRaycast = false;
	UPROPERTY(EditAnywhere, Category="Gravity Gun|Debug", meta = (EditCondition = "DrawDebugRaycast", EditConditionHides, ClampMax = "5.0"))
	float TimerDebugRaycast = 0.5f;
};
