#include "MultiplayerMasterGameInstance.h"
#include "MenuSystem/MainMenu.h"


UMultiplayerMasterGameInstance::UMultiplayerMasterGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_Menu"));
	if (!MenuBPClass.Class)
		return;

	MenuWidgetClass = MenuBPClass.Class;
}

void UMultiplayerMasterGameInstance::Init()
{
	if (!MenuWidgetClass)
		return;

	UE_LOG(LogTemp, Warning, TEXT("MenuWidgetClass : %s"), *MenuWidgetClass->GetName());
}

void UMultiplayerMasterGameInstance::LoadMenu()
{
	if (!MenuWidgetClass)
		return;

	MainMenuWidget = CreateWidget<UMainMenu>(this, MenuWidgetClass);
	if (!MainMenuWidget)
		return;

	MainMenuWidget->Show();
	MainMenuWidget->SetMenuInterface(this);
}

void UMultiplayerMasterGameInstance::Host()
{		
	if (MainMenuWidget != nullptr)
	{
		MainMenuWidget->Hide();
	}

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
