#pragma once

#include "UObject/Interface.h"
#include "MenuInterface.generated.h"


UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/** 
 * Menu interface need to implement menu logic in game
 */
class MULTIPLAYERMASTER_API IMenuInterface
{
	GENERATED_BODY()


public:
	virtual void Host() = 0;
};
