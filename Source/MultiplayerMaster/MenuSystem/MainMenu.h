#pragma once

#include "BaseMenuWidget.h"
#include "MainMenu.generated.h"


/**
 *  Server data struct
 */
USTRUCT()
struct FServerData
{
	GENERATED_BODY()


	FString Name;

	uint16 CurrentPlayers;

	uint16 MaxPlayers;

	FString HostUsername;

	int32 PingInMs;
};

/**
 * Main Menu Widget
 */
UCLASS()
class MULTIPLAYERMASTER_API UMainMenu
	: public UBaseMenuWidget
{
	GENERATED_BODY()


private:
	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = MainMenu)
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = MainMenu)
	class UWidget* MainMenuWidget;

	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = MainMenu)
	class UButton* HostMenuButton;

	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = MainMenu)
	class UButton* JoinMenuButton;

	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = MainMenu)
	class UButton* QuitGameButton;

	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = HostMenu)
	class UWidget* HostMenuWidget;

	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = HostMenu)
	class UEditableTextBox* ServerNameTextBox;

	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = HostMenu)
	class UButton* HostButton;

	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = HostMenu)
	class UButton* HostMenuCancelButton;

	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = JoinMenu)
	class UWidget* JoinMenuWidget;

	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = JoinMenu)
	class UPanelWidget* ServerList;

	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = JoinMenu)
	class UWidget* SessionsWaitWidget;

	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = JoinMenu)
	class UButton* JoinButton;

	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = JoinMenu)
	class UButton* RefreshButton;

	UPROPERTY(VisibleAnywhere, Meta = (BindWidget), Category = JoinMenu)
	class UButton* JoinMenuCancelButton;


private:
	TSubclassOf<class UUserWidget> ServerRowClass;
	TOptional<uint32> SelectedIndex;


private:
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void RefreshServerList();

	UFUNCTION()
	void ReturnToMainMenu();

	UFUNCTION()
	void QuitGame();


private:
	void UpdateChildren();


public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);
	void SetServerList(const TArray<FServerData>& ServerDatas);
	void SelectIndex(uint32 Index);


protected:
	virtual bool Initialize() override;
};
