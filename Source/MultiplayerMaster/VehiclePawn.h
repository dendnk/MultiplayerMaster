#pragma once

#include "GameFramework/Pawn.h"
#include "VehiclePawn.generated.h"

USTRUCT()
struct FVehicleMove
{
	GENERATED_BODY()

	UPROPERTY()
	float Throttle;

	UPROPERTY()
	float SteeringThrow;

	UPROPERTY()
	float DeltaTime;

	UPROPERTY()
	float Time;
};

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


UCLASS()
class MULTIPLAYERMASTER_API AVehiclePawn : public APawn
{
	GENERATED_BODY()


	/** The mass of vehicle (kg) */
	UPROPERTY(EditAnywhere)
	float Mass = 1000.f;

	/** The force applied to the vehicle when throttle is fully down (Newtons) */
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000.f;

	/** Higher means more drag (kg/m). 16 means that maximum velocity = 25 m/s */
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;

	/** Higher means more rolling resistance. Ordinary car tires on concrete = 0.01 to 0.015. Dimensionless */
	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficient = 0.015f;

	/** Minimum radius of the car turning circle at full lock (m) */
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;

	/** The number of degrees rotated per second at full control throw (Degrees/s) */
	UPROPERTY(EditAnywhere)
	float MaxDegreesPerSecond = 90.f;

	UPROPERTY(ReplicatedUsing = OnRep_VehicleState)
	FVehicleState ServerVehicleState;

	FVector Velocity;

	float Throttle;
	float SteeringThrow;

	TArray<FVehicleMove> UnacknowledgedMoves;


private:
	FVehicleMove CreateMove(const float DeltaTime) const;
	void SimulateMove(const FVehicleMove& Move);
	void ClearAcknowledgedMoves(const FVehicleMove LastMove);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSendMove(const FVehicleMove Move);

	UFUNCTION()
	void OnRep_VehicleState();

	FVector GetAirResistance() const;
	FVector GetRollingResistance() const;

	void ApplyRotation(const float DeltaTime, const float InSteeringThrow);
	void UpdateLocationFromVelocity(const float DeltaTime);

	void MoveForward(const float Value);
	void MoveRight(const float Value);


protected:
	virtual void BeginPlay() override;


public:
	AVehiclePawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
