#include "VehiclePawn.h"

#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"


AVehiclePawn::AVehiclePawn()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AVehiclePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVehiclePawn, ServerVehicleState);
}

void AVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		NetUpdateFrequency = 1;
	}
}

FVehicleMove AVehiclePawn::CreateMove(const float DeltaTime) const
{
	FVehicleMove Move;
	Move.DeltaTime = DeltaTime;
	Move.SteeringThrow = SteeringThrow;
	Move.Throttle = Throttle;
	Move.Time = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

	return Move;
}

void AVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		const FVehicleMove Move = CreateMove(DeltaTime);
		SimulateMove(Move);

		UnacknowledgedMoves.Add(Move);
		ServerSendMove(Move);
	}

	/** We are the server and control of the pawn */
	if (GetLocalRole() == ROLE_Authority && IsLocallyControlled())
	{
		const FVehicleMove Move = CreateMove(DeltaTime);
		ServerSendMove(Move);
	}

	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		SimulateMove(ServerVehicleState.LastMove);
	}
}

void AVehiclePawn::OnRep_VehicleState()
{
	SetActorTransform(ServerVehicleState.Transform);
	Velocity = ServerVehicleState.Velocity;

	ClearAcknowledgedMoves(ServerVehicleState.LastMove);

	for (const auto& Move : UnacknowledgedMoves)
	{
		SimulateMove(Move);
	}
}

void AVehiclePawn::ClearAcknowledgedMoves(const FVehicleMove LastMove)
{
	TArray<FVehicleMove> NewMoves;

	for (const auto& Move : UnacknowledgedMoves)
	{
		if (Move.Time > LastMove.Time)
		{
			NewMoves.Add(Move);
		}
	}

	UnacknowledgedMoves = NewMoves;
}

void AVehiclePawn::SimulateMove(const FVehicleMove& Move)
{
	FVector Force = GetActorForwardVector() * MaxDrivingForce * Move.Throttle;
	Force += GetAirResistance();
	Force += GetRollingResistance();

	const FVector Acceleration = Force / Mass;

	Velocity = Velocity + Acceleration * Move.DeltaTime;

	ApplyRotation(Move.DeltaTime, Move.SteeringThrow);

	UpdateLocationFromVelocity(Move.DeltaTime);
}

void AVehiclePawn::ServerSendMove_Implementation(const FVehicleMove Move)
{
	SimulateMove(Move);

	ServerVehicleState.LastMove = Move;
	ServerVehicleState.Transform = GetActorTransform();
	ServerVehicleState.Velocity = Velocity;
}

bool AVehiclePawn::ServerSendMove_Validate(const FVehicleMove Move)
{
	return (FMath::Abs(Move.Throttle) <= 1)  && (FMath::Abs(Move.SteeringThrow) <= 1);
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

void AVehiclePawn::ApplyRotation(const float DeltaTime, const float InSteeringThrow)
{
	const float DeltaLocation = FVector::DotProduct(GetActorForwardVector(), Velocity) * DeltaTime;
	/* DeltaAngleInRadians  = DeltaLocation / Radius */
	const float RotationAngle = DeltaLocation / MinTurningRadius * InSteeringThrow;
	const FQuat RotationDelta(GetActorUpVector(), RotationAngle);

	Velocity = RotationDelta.RotateVector(Velocity);

	AddActorWorldRotation(RotationDelta);
}

void AVehiclePawn::UpdateLocationFromVelocity(const float DeltaTime)
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

void AVehiclePawn::MoveForward(const float Value)
{
	Throttle = Value;
}

void AVehiclePawn::MoveRight(const float Value)
{
	SteeringThrow = Value;
}
