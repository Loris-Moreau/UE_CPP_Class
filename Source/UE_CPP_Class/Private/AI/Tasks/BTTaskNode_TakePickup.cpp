#include "AI/Tasks/BTTaskNode_TakePickup.h"

#include "AI/Enemy.h"
#include "AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Gameplay/GravityGunComponent.h"


EBTNodeResult::Type UBTTaskNode_TakePickup::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the Character & Controller
	if((EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner())))
	{
		EnemyCharacter = Cast<AEnemy>(EnemyController->GetPawn());
	}
	
	// Check if we've got the character
	if(!EnemyCharacter) return EBTNodeResult::Failed;
	
	// Get the Blackboard Component
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if(!BlackboardComponent) return Failed();

	Pickup = Cast<AActor>(BlackboardComponent->GetValueAsObject(PickupKey.SelectedKeyName));
	if(!Pickup) return Failed();

	// get grav gun comp
	GravityGunComp = EnemyCharacter->FindComponentByClass<UGravityGunComponent>();
	if (!GravityGunComp) return Failed();
	
	// Check if we're close enough
	const FVector EnemyLocation = EnemyCharacter->GetActorLocation();
	const FVector PickupLocation = Pickup->GetActorLocation();
	FVector EnemyToPickup = PickupLocation - EnemyLocation;

	const float RaycastSize = GravityGunComp->GetRaycastSize();
	if (RaycastSize < EnemyToPickup.Size()) return Failed();

	// Add some Variation to the Raycast
	// Pitch 
	RandowAnglePitch = FMath::FRandRange(-RandomAngleAroundRaycast, RandomAngleAroundRaycast);
	EnemyToPickup = EnemyToPickup.RotateAngleAxis(RandowAnglePitch, EnemyCharacter->GetActorRightVector());
	// Yaw 
	RandowAngleYaw = FMath::FRandRange(-RandomAngleAroundRaycast, RandomAngleAroundRaycast);
	EnemyToPickup = EnemyToPickup.RotateAngleAxis(RandowAngleYaw, EnemyCharacter->GetActorUpVector());

	// Compute Check Location
	EnemyToPickup.Normalize();
	FVector CheckLocation = EnemyLocation + (EnemyToPickup * RaycastSize);
	
	// Launch the Ray
	if(GravityGunComp->TryTakePickup(EnemyLocation, CheckLocation)) return Failed();

	// Update Blackboard Value
	BlackboardComponent->SetValueAsBool(EnemyHasPickupKey.SelectedKeyName, true);
	
	return EBTNodeResult::Succeeded;
}

void UBTTaskNode_TakePickup::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	FString PickupName = Pickup ? Pickup->GetName() : "NONE";
	
	Values.Add(FString::Printf(TEXT("Pickup Name %s - Random Angle Pitch %f and Random Angle Yaw %f"),
		*PickupName, RandowAnglePitch, RandowAngleYaw));
	
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
}

FString UBTTaskNode_TakePickup::GetStaticDescription() const
{
	FString Description("invalid");
	// Check if the keys given by the user are valid
	if(CanTakePickupKey.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass() &&
		PickupKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		Description = "Pick Up : " + PickupKey.SelectedKeyName.ToString()
		+ " - Can Take Pickup : " + CanTakePickupKey.SelectedKeyName.ToString()
		+ FString(TEXT(" - Pickup Timer %f"), PickupTimer)
		+ FString(TEXT(" - Random Angle Around Raycast %f"), RandomAngleAroundRaycast);
	}
	
	return FString::Printf(TEXT("%s : %s"), *Super::GetStaticDescription(), *Description);
}

EBTNodeResult::Type UBTTaskNode_TakePickup::Failed()
{
	// Launch Timer
	EnemyController->StartCanTakePickupTimer(PickupTimer);

	return EBTNodeResult::Failed;
}
