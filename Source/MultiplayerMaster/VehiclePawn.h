#pragma once

#include "GameFramework/Pawn.h"
#include "VehicleMovementComponent.h"
#include "VehicleMovementReplicator.h"
#include "VehiclePawn.generated.h"


UCLASS()
class MULTIPLAYERMASTER_API AVehiclePawn
	: public APawn
{
	GENERATED_BODY()


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UVehicleMovementComponent* VehicleMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UVehicleMovementReplicator* VehicleReplicator;


private:
	const static FName VehicleMovementName;
	const static FName VehicleReplicatorName;
	
	void MoveForward(const float Value);
	void MoveRight(const float Value);

	
protected:
	virtual void BeginPlay() override;


public:
	AVehiclePawn(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
