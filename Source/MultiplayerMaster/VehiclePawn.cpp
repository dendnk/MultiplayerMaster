#include "VehiclePawn.h"
#include "GameFramework/GameStateBase.h"


const FName AVehiclePawn::VehicleMovementName = TEXT("Vehicle Movement");
const FName AVehiclePawn::VehicleReplicatorName = TEXT("Vehicle Replicator");

AVehiclePawn::AVehiclePawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	VehicleMovement = CreateDefaultSubobject<UVehicleMovementComponent>(VehicleMovementName);
	VehicleReplicator = CreateDefaultSubobject<UVehicleMovementReplicator>(VehicleReplicatorName);

	bReplicates = true;
	SetReplicatingMovement(false);
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
	if (VehicleMovement == nullptr)
		return;
	
	VehicleMovement->SetThrottle(Value);
}

void AVehiclePawn::MoveRight(const float Value)
{
	if (VehicleMovement == nullptr)
		return;
	
	VehicleMovement->SetSteeringThrow(Value);
}
