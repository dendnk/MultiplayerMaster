#pragma once

#include "GameFramework/Pawn.h"
#include "VehicleMovementComponent.h"
#include "VehiclePawn.generated.h"


USTRUCT()
struct FVehicleState
{
	GENERATED_BODY()

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FVehicleMove LastMove;
};


UCLASS()
class MULTIPLAYERMASTER_API AVehiclePawn
	: public APawn
{
	GENERATED_BODY()


private:
	const static FName VehicleMovementComponentName;


public:
	/**
	 * Vehicle movement component with physics
	 */
	UPROPERTY(EditAnywhere)
	UVehicleMovementComponent* VehicleMovementComponent;

	TArray<FVehicleMove> UnacknowledgedMoves;

	UPROPERTY(ReplicatedUsing = OnRep_VehicleState)
	FVehicleState ServerVehicleState;

private:
	void ClearAcknowledgedMoves(const FVehicleMove LastMove);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSendMove(const FVehicleMove Move);

	UFUNCTION()
	void OnRep_VehicleState();

	void MoveForward(const float Value);
	void MoveRight(const float Value);


protected:
	virtual void BeginPlay() override;


public:
	AVehiclePawn(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
