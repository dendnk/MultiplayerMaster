#pragma once

#include "GameFramework/Actor.h"
#include "PlatformTrigger.generated.h"


/**
 * Platform trigger has links to platform.
 * Activate linked moving platform on overlap.
 * Linked platforms must have property ActiveTriggers == 0.
 */
UCLASS()
class MULTIPLAYERMASTER_API APlatformTrigger
	: public AActor
{
	GENERATED_BODY()


private:
	static FName TriggerVolumeName;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerVolume;

	UPROPERTY(EditAnywhere)
	TArray<class AMovingPlatfrom*> PlatfromsToTrigger;


private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:
	APlatformTrigger(const FObjectInitializer& ObjectInitializer);


protected:
	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaTime) override;
};
