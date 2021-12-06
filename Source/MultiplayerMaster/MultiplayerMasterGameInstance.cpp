#include "MultiplayerMasterGameInstance.h"
#include "MenuSystem/GameMenu.h"
#include "MenuSystem/MainMenu.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"


const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

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
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found susbsystem : %s"), *Subsystem->GetSubsystemName().ToString());

		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerMasterGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerMasterGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerMasterGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMultiplayerMasterGameInstance::OnJoinSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found no susbsystem!"));
	}	
}

void UMultiplayerMasterGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;
	if (SessionInterface.IsValid())
	{
		const auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(NAME_GameSession);
		}
		else
		{
			CreateSession();
		}
	}
}

void UMultiplayerMasterGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid())
		return;

	auto SearchResults = SessionSearch->SearchResults;
	if (SearchResults.Num() == 0 || !SearchResults.IsValidIndex(Index))
	{
		UE_LOG(LogTemp,Warning,TEXT("1) There are no search results from session search to join\n 2) Selected index not valid!"));
		return;
	}
	
	if (MainMenuWidget != nullptr)
	{
		MainMenuWidget->Hide();
	}
	
	SessionInterface->JoinSession(0, NAME_GameSession , SessionSearch->SearchResults[Index]);
}

void UMultiplayerMasterGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid())
		return;

	FString Address;
	
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string!"));
		return;
	}

	auto Engine = GetEngine();
	if (!Engine)
		return;
	
	Engine->AddOnScreenDebugMessage(0, 2.0f, FColor::Blue, FString::Printf(TEXT("Joining to : %s"), *Address));	
	
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController)
		return;
	
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);	
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

void UMultiplayerMasterGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == TEXT("NULL"))
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}
		
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		
		SessionInterface->CreateSession(0, NAME_GameSession , SessionSettings);		
	}
}

void UMultiplayerMasterGameInstance::OnCreateSessionComplete(FName SessionName, bool bSuccess)
{
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session!"));
		return;
	}
	
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

	World->ServerTravel(TEXT("/Game/MultiplayerMaster/Maps/Lobby?listen"));
}

void UMultiplayerMasterGameInstance::OnDestroySessionComplete(FName SessionName, bool bSuccess)
{
	if (bSuccess)
	{
		CreateSession();
	}
}

void UMultiplayerMasterGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Starting find session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());	
	}
}

void UMultiplayerMasterGameInstance::OnFindSessionsComplete(bool bSuccess)
{
	if (bSuccess && SessionSearch.IsValid() && MainMenuWidget != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Finish find session"));
		
		TArray<FServerData> ServerDatas;
		for (const auto& Result : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Session: %s"), *Result.GetSessionIdStr());
			FServerData Data;
			
			Data.HostUsername = Result.Session.OwningUserName;
			Data.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - Result.Session.NumOpenPublicConnections;
			Data.PingInMs = Result.PingInMs;
			FString ServerName;
			if (Result.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
				Data.Name = ServerName;
			}
			else
			{
				Data.Name = TEXT("Could not find name!");
			}
			
			ServerDatas.Add(Data);
		}

		MainMenuWidget->SetServerList(ServerDatas);
	}
}

void UMultiplayerMasterGameInstance::LoadMenuWidget()
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
