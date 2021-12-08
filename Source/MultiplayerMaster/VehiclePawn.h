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

	/** The number of degrees rotated per second at full control throw (Degrees/s) */
	UPROPERTY(EditAnywhere)
	float MaxDegreesPerSecond = 90.f;

	FVector Velocity;

	float Throttle;
	float SteeringThrow;


public:
	AVehiclePawn();

private:
	void MoveForward(float Value);
	void MoveRight(float Value);

	FVector GetResistance() const;
	void ApplyRotation(float DeltaTime);
	void UpdateLocationFromVelocity(float DeltaTime);


protected:
	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
