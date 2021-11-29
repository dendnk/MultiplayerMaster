#pragma once

#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "BaseMenuWidget.generated.h"

/**
 * Base menu widget - abstract base class for menu widgets
 */
UCLASS()
class MULTIPLAYERMASTER_API UBaseMenuWidget 
	: public UUserWidget
{
	GENERATED_BODY()
	

public:
	void Show();
	void Hide();

	void SetMenuInterface(IMenuInterface* InMenuInterface);


protected:
	IMenuInterface* MenuInterface;
};
