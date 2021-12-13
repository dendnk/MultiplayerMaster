#pragma once

#include "Components/ActorComponent.h"
#include "VehicleMovementComponent.h"
#include "VehicleMovementReplicator.generated.h"


USTRUCT()
struct FVehicleState
{
	GENERATED_BODY()

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FVehicleMove LastMove;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERMASTER_API UVehicleMovementReplicator
	: public UActorComponent
{
	GENERATED_BODY()


	UPROPERTY()
	UVehicleMovementComponent* VehicleMovementComponent;


public:
	UPROPERTY(ReplicatedUsing = OnRep_VehicleState)
	struct FVehicleState ServerVehicleState;

	TArray<FVehicleMove> UnacknowledgedMoves;


private:
	void ClearAcknowledgedMoves(const FVehicleMove LastMove);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSendMove(const FVehicleMove Move);

	UFUNCTION()
	void OnRep_VehicleState();


protected:
	virtual void BeginPlay() override;


public:
	UVehicleMovementReplicator();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
