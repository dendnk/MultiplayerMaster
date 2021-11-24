#include "MultiplayerMasterGameInstance.h"


UMultiplayerMasterGameInstance::UMultiplayerMasterGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Constructor"));
}

void UMultiplayerMasterGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"));
}

void UMultiplayerMasterGameInstance::Host()
{		
	if (!GEngine)
		return; 

	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Blue, TEXT("Hosting"));

	auto World = GetWorld();

	if (!World)
		return;

	World->ServerTravel(TEXT("/Game/MultiplayerMaster/Maps/Map?listen"));
}

void UMultiplayerMasterGameInstance::Join(const FString& InIPAddress)
{
	if (!GEngine)
		return;

	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Blue, FString::Printf(TEXT("Joining to : %s"), *InIPAddress));	

	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (!PlayerController)
		return;

	PlayerController->ClientTravel(InIPAddress, ETravelType::TRAVEL_Absolute);
}
