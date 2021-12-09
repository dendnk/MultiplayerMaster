#pragma once

#include "GameFramework/Pawn.h"
#include "VehiclePawn.generated.h"

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

	
	FVector Velocity;

	float Throttle;
	float SteeringThrow;
	

private:
	void MoveForward(float Value);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveForward(float Value);

	void MoveRight(float Value);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveRight(float Value);
	
	FVector GetAirResistance() const;
	FVector GetRollingResistance() const;

	void ApplyRotation(float DeltaTime);
	void UpdateLocationFromVelocity(float DeltaTime);


protected:
	virtual void BeginPlay() override;


public:
	AVehiclePawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
