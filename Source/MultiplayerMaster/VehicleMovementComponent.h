#pragma once

#include "Components/ActorComponent.h"
#include "VehicleMovementComponent.generated.h"


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

	bool IsValid() const
	{
		return FMath::Abs(Throttle) <= 1 && FMath::Abs(SteeringThrow) <= 1;
	}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERMASTER_API UVehicleMovementComponent
	: public UActorComponent
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
	float MinTurningRadius = 5.f;

	/** The number of degrees rotated per second at full control throw (Degrees/s) */
	UPROPERTY(EditAnywhere)
	float MaxDegreesPerSecond = 90.f;

	FVector Velocity;

	float Throttle;
	float SteeringThrow;

	FVehicleMove LastMove;


public:
	void SimulateMove(const FVehicleMove& Move);

	FVector GetVelocity() const { return Velocity; }
	void SetVelocity(const FVector& NewVelocity) { this->Velocity = NewVelocity; }
	void SetThrottle(const float NewThrottle) {	this->Throttle = NewThrottle; }
	void SetSteeringThrow(const float NewSteeringThrow) { this->SteeringThrow = NewSteeringThrow; }
	FVehicleMove GetLastMove() const { return LastMove; }


private:
	FVehicleMove CreateMove(const float DeltaTime) const;

	FVector GetAirResistance() const;
	FVector GetRollingResistance() const;

	void ApplyRotation(const float DeltaTime, const float InSteeringThrow);
	void UpdateLocationFromVelocity(const float DeltaTime);


protected:
	virtual void BeginPlay() override;


public:
	UVehicleMovementComponent(const FObjectInitializer& ObjectInitializer);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
