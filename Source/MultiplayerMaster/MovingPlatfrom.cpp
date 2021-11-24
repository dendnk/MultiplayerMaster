#include "MovingPlatfrom.h"

AMovingPlatfrom::AMovingPlatfrom(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatfrom::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);

		OriginWorldLocation = GetActorLocation();
		TargetWorldLocation = GetTransform().TransformPosition(TargetLocation);
	}
}

void AMovingPlatfrom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActiveTriggers > 0)
	{
		if (HasAuthority())
		{
			FVector Location = GetActorLocation();

			if (Location.Equals(TargetWorldLocation, EqualTolerance))
			{
				// Swap Target and Origin Locations
				OriginWorldLocation = OriginWorldLocation + TargetWorldLocation;
				TargetWorldLocation = OriginWorldLocation - TargetWorldLocation;
				OriginWorldLocation -= TargetWorldLocation;
			}

			FVector Direction = (TargetWorldLocation - Location).GetSafeNormal();
			Location += Speed * DeltaTime * Direction;
			SetActorLocation(Location);
		}
	}	
}

void AMovingPlatfrom::AddActiveTrigger()
{
	++ActiveTriggers;
}

void AMovingPlatfrom::RemoveActiveTrigger()
{
	if (ActiveTriggers > 0)
	{
		--ActiveTriggers;
	}
}
