#pragma once

#include "BaseMenuWidget.h"
#include "GameMenu.generated.h"


/**
 * Game Menu Widget used for calling menu in game
 */
UCLASS()
class MULTIPLAYERMASTER_API UGameMenu
	: public UBaseMenuWidget
{
	GENERATED_BODY()


	UPROPERTY(Meta = (BindWidget))
	class UButton* ReturnToGameButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* MainMenuButton;


private:
	UFUNCTION()
	void ReturnToGame();

	UFUNCTION()
	void GoToMainMenu();


protected:
	virtual bool Initialize() override;
};
