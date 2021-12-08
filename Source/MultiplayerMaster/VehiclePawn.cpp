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
	Force += GetResistance();

	const FVector Acceleration = Force / Mass;

	Velocity = Velocity + Acceleration * DeltaTime;

	ApplyRotation(DeltaTime);

	UpdateLocationFromVelocity(DeltaTime);
}

FVector AVehiclePawn::GetResistance() const
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

void AVehiclePawn::ApplyRotation(float DeltaTime)
{
	const float RotationAngle = MaxDegreesPerSecond * DeltaTime * SteeringThrow;
	const FQuat RotationDelta(GetActorUpVector(), FMath::DegreesToRadians(RotationAngle));

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
}

void AVehiclePawn::MoveRight(float Value)
{
	SteeringThrow = Value;
}
