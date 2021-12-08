#pragma once

#include "GameFramework/Pawn.h"
#include "VehiclePawn.generated.h"

UCLASS()
class MULTIPLAYERMASTER_API AVehiclePawn : public APawn
{
	GENERATED_BODY()

public:
	AVehiclePawn();

	
protected:
	virtual void BeginPlay() override;

	
public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
