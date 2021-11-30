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
	class UPanelWidget* ServerList;

	UPROPERTY(Meta = (BindWidget))
	class UButton* QuitGameButton;


private:
	TSubclassOf<class UUserWidget> ServerRowClass;
	TOptional<uint32> SelectedIndex;	


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


public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);
	void SetServerList(const TArray<FString>& ServerNames);
	void SelectIndex(uint32 Index);
	

protected:
	virtual bool Initialize() override;
};
