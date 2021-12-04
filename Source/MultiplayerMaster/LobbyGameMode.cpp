#include "LobbyGameMode.h"


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (GetNumPlayers() >= 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("You have reached 3 players!"));
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}
