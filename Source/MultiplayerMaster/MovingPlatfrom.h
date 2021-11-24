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
	float Speed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MakeEditWidget = true))
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EqualTolerance = 5.f;


private:
	UPROPERTY(EditAnywhere)
	int32 ActiveTriggers = 1;	


public:
	AMovingPlatfrom(const FObjectInitializer& ObjectInitializer);


public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;	

	void AddActiveTrigger();
	void RemoveActiveTrigger();

private:
	FVector OriginWorldLocation;
	FVector TargetWorldLocation;
};
