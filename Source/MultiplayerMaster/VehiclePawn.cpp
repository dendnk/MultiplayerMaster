#include "VehiclePawn.h"

#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"


const FName AVehiclePawn::VehicleMovementComponentName = TEXT("Vehicle Component");

AVehiclePawn::AVehiclePawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	VehicleMovementComponent = CreateDefaultSubobject<UVehicleMovementComponent>(VehicleMovementComponentName);

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

void AVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (VehicleMovementComponent == nullptr)
		return;
	
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		const FVehicleMove Move = VehicleMovementComponent->CreateMove(DeltaTime);
		VehicleMovementComponent->SimulateMove(Move);

		UnacknowledgedMoves.Add(Move);
		ServerSendMove(Move);
	}

	/** We are the server and control of the pawn */
	if (GetLocalRole() == ROLE_Authority && IsLocallyControlled())
	{
		const FVehicleMove Move = VehicleMovementComponent->CreateMove(DeltaTime);
		ServerSendMove(Move);
	}

	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		VehicleMovementComponent->SimulateMove(ServerVehicleState.LastMove);
	}
}

void AVehiclePawn::OnRep_VehicleState()
{
	SetActorTransform(ServerVehicleState.Transform);
	VehicleMovementComponent->SetVelocity(ServerVehicleState.Velocity);

	ClearAcknowledgedMoves(ServerVehicleState.LastMove);

	for (const auto& Move : UnacknowledgedMoves)
	{
		VehicleMovementComponent->SimulateMove(Move);
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

void AVehiclePawn::ServerSendMove_Implementation(const FVehicleMove Move)
{
	VehicleMovementComponent->SimulateMove(Move);

	ServerVehicleState.LastMove = Move;
	ServerVehicleState.Transform = GetActorTransform();
	ServerVehicleState.Velocity = VehicleMovementComponent->GetVelocity();
}

bool AVehiclePawn::ServerSendMove_Validate(const FVehicleMove Move)
{
	return (FMath::Abs(Move.Throttle) <= 1)  && (FMath::Abs(Move.SteeringThrow) <= 1);
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
	VehicleMovementComponent->SetThrottle(Value);
}

void AVehiclePawn::MoveRight(const float Value)
{
	VehicleMovementComponent->SetSteeringThrow(Value);
}
