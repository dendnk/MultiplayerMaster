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
		ClientTick(DeltaTime);
	}
}

void UVehicleMovementReplicator::OnRep_VehicleState()
{
	switch (GetOwnerRole())
	{
	case ROLE_AutonomousProxy :
		AutonomousProxy_OnRep_VehicleState();
		break;
	case ROLE_SimulatedProxy :
		SimulatedProxy_OnRep_VehicleState();
		break;
	default :
		break;
	}
}

void UVehicleMovementReplicator::AutonomousProxy_OnRep_VehicleState()
{
	if (VehicleMovementComponent == nullptr)
		return;
	
	GetOwner()->SetActorTransform(ServerVehicleState.Transform);
	VehicleMovementComponent->SetVelocity(ServerVehicleState.Velocity);

	ClearAcknowledgedMoves(ServerVehicleState.LastMove);

	for (const auto& Move : UnacknowledgedMoves)
	{
		VehicleMovementComponent->SimulateMove(Move);
	}
}

void UVehicleMovementReplicator::SimulatedProxy_OnRep_VehicleState()
{
	if (VehicleMovementComponent == nullptr)
		return;
	
	ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0.f;

	ClientStartTransform = GetOwner()->GetActorTransform();
	ClientStartVelocity = ServerVehicleState.Velocity;
}

void UVehicleMovementReplicator::UpdateServerState(const FVehicleMove& Move)
{
	ServerVehicleState.LastMove = Move;
	ServerVehicleState.Transform = GetOwner()->GetActorTransform();
	ServerVehicleState.Velocity = VehicleMovementComponent->GetVelocity();
}

void UVehicleMovementReplicator::ClientTick(float DeltaTime)
{	
	ClientTimeSinceUpdate += DeltaTime;
	
	if (ClientTimeSinceUpdate < KINDA_SMALL_NUMBER)
		return;

	if (VehicleMovementComponent == nullptr)
		return;

	const float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdates;
	FHermiteCubicSpline Spline = CreateSpline();
	
	InterpolateSpline(Spline, LerpRatio);

	InterpolateVelocity(Spline, LerpRatio);

	InterpolateRotation(LerpRatio);
}

FHermiteCubicSpline UVehicleMovementReplicator::CreateSpline()
{
	FHermiteCubicSpline Spline;
	Spline.TargetLocation = ServerVehicleState.Transform.GetLocation();
	Spline.StartLocation = ClientStartTransform.GetLocation();
	Spline.StartDerivative = ClientStartVelocity * VelocityToDerivative();
	Spline.TargetDerivative = ServerVehicleState.Velocity * VelocityToDerivative();

	return Spline;
}

float UVehicleMovementReplicator::VelocityToDerivative() const
{
	return ClientTimeBetweenLastUpdates * 100; /* *100 Convert to cm from m */
}

void UVehicleMovementReplicator::InterpolateSpline(const FHermiteCubicSpline& Spline, float LerpRatio) const
{
	const FVector NewLocation = Spline.InterpolateLocation(LerpRatio);
	
	GetOwner()->SetActorLocation(NewLocation);
}

void UVehicleMovementReplicator::InterpolateVelocity(const FHermiteCubicSpline& Spline, const float LerpRatio) const
{
	const FVector NewDerivative = Spline.InterpolateDerivative(LerpRatio);
	const FVector NewVelocity = NewDerivative / VelocityToDerivative();
	
	VehicleMovementComponent->SetVelocity(NewVelocity);
}

void UVehicleMovementReplicator::InterpolateRotation(const float LerpRatio) const
{
	const FQuat TargetRotation = ServerVehicleState.Transform.GetRotation();
	const FQuat StartRotation = ClientStartTransform.GetRotation();
	const FQuat NewRotation = FQuat::Slerp(StartRotation, TargetRotation, LerpRatio);
	
	GetOwner()->SetActorRotation(NewRotation);
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
