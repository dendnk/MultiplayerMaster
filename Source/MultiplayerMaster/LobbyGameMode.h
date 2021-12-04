#pragma once

#include "MultiplayerMasterGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 *  Lobby Game Mode 
 */
UCLASS()
class MULTIPLAYERMASTER_API ALobbyGameMode : public AMultiplayerMasterGameMode
{
	GENERATED_BODY()


public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout( AController* Exiting ) override;
};
