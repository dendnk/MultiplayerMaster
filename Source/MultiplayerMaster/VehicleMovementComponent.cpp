#include "VehicleMovementComponent.h"
#include "GameFramework/GameStateBase.h"


UVehicleMovementComponent::UVehicleMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

FVehicleMove UVehicleMovementComponent::CreateMove(const float DeltaTime) const
{
	FVehicleMove Move;
	Move.DeltaTime = DeltaTime;
	Move.SteeringThrow = SteeringThrow;
	Move.Throttle = Throttle;
	Move.Time = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

	return Move;
}

void UVehicleMovementComponent::SimulateMove(const FVehicleMove& Move)
{
	FVector Force = GetOwner()->GetActorForwardVector() * MaxDrivingForce * Move.Throttle;
	Force += GetAirResistance();
	Force += GetRollingResistance();

	const FVector Acceleration = Force / Mass;

	Velocity = Velocity + Acceleration * Move.DeltaTime;

	ApplyRotation(Move.DeltaTime, Move.SteeringThrow);

	UpdateLocationFromVelocity(Move.DeltaTime);
}

FVector UVehicleMovementComponent::GetAirResistance() const
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector UVehicleMovementComponent::GetRollingResistance() const
{
	const auto World = GetWorld();
	if (World == nullptr)
		return FVector::ZeroVector;

	const float AccelerationDueToGravity =  -World->GetGravityZ() / 100.f;
	const float NormaForceMagnitude = Mass * AccelerationDueToGravity;

	return -Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormaForceMagnitude;
}

void UVehicleMovementComponent::ApplyRotation(const float DeltaTime, const float InSteeringThrow)
{
	const float DeltaLocation = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
	/* DeltaAngleInRadians  = DeltaLocation / Radius */
	const float RotationAngle = DeltaLocation / MinTurningRadius * InSteeringThrow;
	const FQuat RotationDelta(GetOwner()->GetActorUpVector(), RotationAngle);

	Velocity = RotationDelta.RotateVector(Velocity);

	GetOwner()->AddActorWorldRotation(RotationDelta);
}

void UVehicleMovementComponent::UpdateLocationFromVelocity(const float DeltaTime)
{
	const FVector Translation = Velocity * 100.f * DeltaTime;

	FHitResult HitResultInfo;
	GetOwner()->AddActorWorldOffset(Translation, true, &HitResultInfo);
	if (HitResultInfo.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
	}
}

void UVehicleMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UVehicleMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
