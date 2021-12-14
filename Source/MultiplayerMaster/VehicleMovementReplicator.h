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

	float ClientTimeSinceUpdate;
	float ClientTimeBetweenLastUpdates;
	FTransform ClientStartTransform;
	FVector ClientStartVelocity;


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
	void AutonomousProxy_OnRep_VehicleState();
	void SimulatedProxy_OnRep_VehicleState();

	void UpdateServerState(const FVehicleMove& Move);

	void ClientTick(float DeltaTime);


protected:
	virtual void BeginPlay() override;


public:
	UVehicleMovementReplicator();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
