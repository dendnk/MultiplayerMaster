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

struct FHermiteCubicSpline
{
	FVector StartLocation;
	FVector StartDerivative;
	FVector TargetLocation;
	FVector TargetDerivative;

	FVector InterpolateLocation(const float LerpRatio) const
	{
		return FMath::CubicInterp(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	}
	
	FVector InterpolateDerivative(const float LerpRatio) const
	{
		return FMath::CubicInterpDerivative(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERMASTER_API UVehicleMovementReplicator
	: public UActorComponent
{
	GENERATED_BODY()


	UPROPERTY()
	UVehicleMovementComponent* VehicleMovementComponent;

	UPROPERTY()
	USceneComponent* MeshOffsetRoot;

	float ClientTimeSinceUpdate;
	float ClientTimeBetweenLastUpdates;
	FTransform ClientStartTransform;
	FVector ClientStartVelocity;

	float ClientSimulatedTime;


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

	FHermiteCubicSpline CreateSpline();
	float VelocityToDerivative() const;
	void InterpolateLocation(const FHermiteCubicSpline& Spline, float LerpRatio) const;
	void InterpolateVelocity(const FHermiteCubicSpline& Spline, float LerpRatio) const;
	void InterpolateRotation(float LerpRatio) const;

	UFUNCTION(BlueprintCallable)
	void SetMeshOffsetRoot(USceneComponent* Root) { MeshOffsetRoot = Root; }


protected:
	virtual void BeginPlay() override;


public:
	UVehicleMovementReplicator();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
