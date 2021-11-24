#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"
#include "MovingPlatfrom.h"


FName APlatformTrigger::TriggerVolumeName = FName(TEXT("TriggerVolume"));


APlatformTrigger::APlatformTrigger(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{ 	
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TriggerVolumeName);
	SetRootComponent(TriggerVolume);

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnBeginOverlap);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnEndOverlap);

	PrimaryActorTick.bCanEverTick = true;
}

void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();	
}

void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlatformTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (auto& Platform : PlatfromsToTrigger)
	{
		Platform->AddActiveTrigger();
	}
}

void APlatformTrigger::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (auto& Platform : PlatfromsToTrigger)
	{
		Platform->RemoveActiveTrigger();
	}
}
