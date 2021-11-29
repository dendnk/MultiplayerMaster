#pragma once

#include "BaseMenuWidget.h"
#include "MainMenu.generated.h"


/**
 * Main Menu Widget
 */
UCLASS()
class MULTIPLAYERMASTER_API UMainMenu 
	: public UBaseMenuWidget
{
	GENERATED_BODY()


private:
	UPROPERTY(Meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(Meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;	

	UPROPERTY(Meta = (BindWidget))
	class UWidget* MainMenuWidget;

	UPROPERTY(Meta = (BindWidget))
	class UWidget* JoinMenuWidget;

	UPROPERTY(Meta = (BindWidget))
	class UButton* JoinMenuButton;
	
	UPROPERTY(Meta = (BindWidget))
	class UButton* ReturnToMainMenuButton;

	UPROPERTY(Meta = (BindWidget))
	class UEditableTextBox* IPAddressField;

	UPROPERTY(Meta = (BindWidget))
	class UButton* QuitGameButton;


private:
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void ReturnToMainMenu();

	UFUNCTION()
	void QuitGame();


protected:
	virtual bool Initialize() override;
};
