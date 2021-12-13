#include "VehicleMovementReplicator.h"
#include "Net/UnrealNetwork.h"


UVehicleMovementReplicator::UVehicleMovementReplicator()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UVehicleMovementReplicator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVehicleMovementReplicator, ServerVehicleState);
}

void UVehicleMovementReplicator::BeginPlay()
{
	Super::BeginPlay();

	VehicleMovementComponent = GetOwner()->FindComponentByClass<UVehicleMovementComponent>();
}

void UVehicleMovementReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (VehicleMovementComponent == nullptr)
		return;

	const FVehicleMove LastMove = VehicleMovementComponent->GetLastMove();

	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		UnacknowledgedMoves.Add(LastMove);
		ServerSendMove(LastMove);
	}

	/** We are the server and control of the pawn */

	bool bIsLocallyControlled = false;
	if (const auto AsPawn = Cast<APawn>(GetOwner()))
	{
		bIsLocallyControlled = AsPawn->IsLocallyControlled();
	}

	if (GetOwnerRole() == ROLE_Authority && bIsLocallyControlled)
	{
		UpdateServerState(LastMove);
	}

	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		VehicleMovementComponent->SimulateMove(ServerVehicleState.LastMove);
	}
}

void UVehicleMovementReplicator::OnRep_VehicleState()
{
	GetOwner()->SetActorTransform(ServerVehicleState.Transform);

	if (VehicleMovementComponent == nullptr)
		return;

	VehicleMovementComponent->SetVelocity(ServerVehicleState.Velocity);

	ClearAcknowledgedMoves(ServerVehicleState.LastMove);

	for (const auto& Move : UnacknowledgedMoves)
	{
		VehicleMovementComponent->SimulateMove(Move);
	}
}

void UVehicleMovementReplicator::UpdateServerState(const FVehicleMove& Move)
{
	ServerVehicleState.LastMove = Move;
	ServerVehicleState.Transform = GetOwner()->GetActorTransform();
	ServerVehicleState.Velocity = VehicleMovementComponent->GetVelocity();
}

void UVehicleMovementReplicator::ClearAcknowledgedMoves(const FVehicleMove LastMove)
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

void UVehicleMovementReplicator::ServerSendMove_Implementation(const FVehicleMove Move)
{
	if (VehicleMovementComponent == nullptr)
		return;

	VehicleMovementComponent->SimulateMove(Move);

	UpdateServerState(Move);
}

bool UVehicleMovementReplicator::ServerSendMove_Validate(const FVehicleMove Move)
{
	return (FMath::Abs(Move.Throttle) <= 1)  && (FMath::Abs(Move.SteeringThrow) <= 1);
}