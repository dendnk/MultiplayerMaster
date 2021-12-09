#include "VehiclePawn.h"


AVehiclePawn::AVehiclePawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
}

void AVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle;
	Force += GetAirResistance();
	Force += GetRollingResistance();

	const FVector Acceleration = Force / Mass;

	Velocity = Velocity + Acceleration * DeltaTime;

	ApplyRotation(DeltaTime);

	UpdateLocationFromVelocity(DeltaTime);
}

FVector AVehiclePawn::GetAirResistance() const
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector AVehiclePawn::GetRollingResistance() const
{
	const auto World = GetWorld();
	if (World == nullptr)
		return FVector::ZeroVector;

	const float AccelerationDueToGravity =  -World->GetGravityZ() / 100.f;
	const float NormaForceMagnitude = Mass * AccelerationDueToGravity;

	return -Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormaForceMagnitude;
}

void AVehiclePawn::ApplyRotation(float DeltaTime)
{
	const float DeltaLocation = FVector::DotProduct(GetActorForwardVector(), Velocity) * DeltaTime;
	/* DeltaAngleInRadians  = DeltaLocation / Radius */
	const float RotationAngle = DeltaLocation / MinTurningRadius * SteeringThrow;
	const FQuat RotationDelta(GetActorUpVector(), RotationAngle);

	Velocity = RotationDelta.RotateVector(Velocity);

	AddActorWorldRotation(RotationDelta);
}

void AVehiclePawn::UpdateLocationFromVelocity(float DeltaTime)
{
	const FVector Translation = Velocity * 100.f * DeltaTime;

	FHitResult HitResultInfo;
	AddActorWorldOffset(Translation, true, &HitResultInfo);
	if (HitResultInfo.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
	}
}

void AVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (IsValid(PlayerInputComponent))
	{
		PlayerInputComponent->BindAxis("MoveForward", this, &AVehiclePawn::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &AVehiclePawn::MoveRight);
	}
}

void AVehiclePawn::MoveForward(float Value)
{
	Throttle = Value;
	ServerMoveForward(Value);
}

void AVehiclePawn::ServerMoveForward_Implementation(float Value)
{
	Throttle = Value;
}

bool AVehiclePawn::ServerMoveForward_Validate(float Value)
{
	return FMath::Abs(Value) <= 1;
}

void AVehiclePawn::MoveRight(float Value)
{
	SteeringThrow = Value;
	ServerMoveRight(Value);
}

void AVehiclePawn::ServerMoveRight_Implementation(float Value)
{
	SteeringThrow = Value;
}

bool AVehiclePawn::ServerMoveRight_Validate(float Value)
{
	return FMath::Abs(Value) <= 1;
}
