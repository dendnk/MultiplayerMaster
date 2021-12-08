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
}

void AVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

