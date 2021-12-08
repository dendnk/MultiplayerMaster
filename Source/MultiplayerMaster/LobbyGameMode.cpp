#include "LobbyGameMode.h"

#include "MultiplayerMasterGameInstance.h"


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (GetNumPlayers() >= 2)
	{
		GetWorldTimerManager().SetTimer(StartGameTimer, this, &ALobbyGameMode::StartGame, 10.f);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void ALobbyGameMode::StartGame()
{
	if (GetNumPlayers() >= 2)
	{
		auto GameInstance = Cast<UMultiplayerMasterGameInstance>(GetGameInstance());
		if (GameInstance == nullptr)
			return;

		GameInstance->StartSession();

		const auto World = GetWorld();
		if (World != nullptr)
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(TEXT("/Game/MultiplayerMaster/Maps/Map?listen"));
		}
	}
}
