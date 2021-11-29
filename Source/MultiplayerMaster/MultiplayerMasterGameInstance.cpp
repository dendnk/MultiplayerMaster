#include "MultiplayerMasterGameInstance.h"
#include "MenuSystem/GameMenu.h"
#include "MenuSystem/MainMenu.h"


UMultiplayerMasterGameInstance::UMultiplayerMasterGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_Menu"));
	if (!MenuBPClass.Class)
		return;

	static ConstructorHelpers::FClassFinder<UUserWidget> GameMenuBPClass(TEXT("/Game/MenuSystem/WBP_GameMenu"));
	if (!GameMenuBPClass.Class)
		return;

	MenuWidgetClass = MenuBPClass.Class;
	GameMenuWidgetClass = GameMenuBPClass.Class;
}

void UMultiplayerMasterGameInstance::Init()
{
	if (!MenuWidgetClass || !GameMenuWidgetClass)
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

void UMultiplayerMasterGameInstance::LoadGameMenu()
{
	if (!GameMenuWidgetClass)
		return;

	GameMenuWidget = CreateWidget<UGameMenu>(this, GameMenuWidgetClass);
	if (!GameMenuWidget)
		return;

	GameMenuWidget->Show();
	GameMenuWidget->SetMenuInterface(this);
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
	if (MainMenuWidget != nullptr)
	{
		MainMenuWidget->Hide();
	}

	if (!GEngine)
		return;

	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Blue, FString::Printf(TEXT("Joining to : %s"), *InIPAddress));	

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController)
		return;

	PlayerController->ClientTravel(InIPAddress, ETravelType::TRAVEL_Absolute);
}

void UMultiplayerMasterGameInstance::LoadMainMenu()
{	
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController)
		return;

	if (PlayerController->HasAuthority())
	{
		auto World = GetWorld();
		if (!World)
			return;

		World->ServerTravel(TEXT("/Game/MenuSystem/MainMenu"));
	}
	else
	{
		PlayerController->ClientTravel(TEXT("/Game/MenuSystem/MainMenu"), ETravelType::TRAVEL_Absolute);
	}	   
}
