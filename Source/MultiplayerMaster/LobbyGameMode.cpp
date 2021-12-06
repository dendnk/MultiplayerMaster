#include "LobbyGameMode.h"


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (GetNumPlayers() >= 3)
	{
		const auto World = GetWorld();
		if (World != nullptr)
		{
			bUseSeamlessTravel = true; 
			World->ServerTravel(TEXT("/Game/MultiplayerMaster/Maps/Map?listen"));			
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}
