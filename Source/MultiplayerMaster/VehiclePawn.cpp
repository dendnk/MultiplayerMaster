#include "VehiclePawn.h"
#include "GameFramework/GameStateBase.h"


const FName AVehiclePawn::VehicleMovementComponentName = TEXT("Vehicle Movement Component");
const FName AVehiclePawn::VehicleMovementReplicatorName = TEXT("Vehicle Movement Replicator Component");

AVehiclePawn::AVehiclePawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	VehicleMovementComponent = CreateDefaultSubobject<UVehicleMovementComponent>(VehicleMovementComponentName);
	VehicleMovementReplicator = CreateDefaultSubobject<UVehicleMovementReplicator>(VehicleMovementReplicatorName);

	bReplicates = true;
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
	VehicleMovementComponent->SetThrottle(Value);
}

void AVehiclePawn::MoveRight(const float Value)
{
	VehicleMovementComponent->SetSteeringThrow(Value);
}
