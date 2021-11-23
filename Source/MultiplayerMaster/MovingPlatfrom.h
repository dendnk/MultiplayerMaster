#pragma once

#include "Engine/StaticMeshActor.h"
#include "MovingPlatfrom.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERMASTER_API AMovingPlatfrom 
	: public AStaticMeshActor
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MakeEditWidget = true))
	FVector TargetLocation;


public:
	AMovingPlatfrom(const FObjectInitializer& ObjectInitializer);


public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;	
};
