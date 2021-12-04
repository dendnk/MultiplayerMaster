#pragma once

#include "UObject/Interface.h"
#include "MenuInterface.generated.h"


UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/** 
 * Menu interface need to be implemented in game (in game instance)
 */
class MULTIPLAYERMASTER_API IMenuInterface
{
	GENERATED_BODY()


public:
	virtual void Host(FString ServerName) = 0;
	virtual void Join(uint32 Index) = 0;
	virtual void LoadMainMenu() = 0;
	virtual void RefreshServerList() = 0;
};
