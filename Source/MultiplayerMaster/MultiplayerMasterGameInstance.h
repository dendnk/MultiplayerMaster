#pragma once

#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MenuSystem/MenuInterface.h"
#include "MultiplayerMasterGameInstance.generated.h"

/**
 * Game Instance has logic for hosting/joining and widgets.
 */
UCLASS()
class MULTIPLAYERMASTER_API UMultiplayerMasterGameInstance 
	: public UGameInstance
	, public IMenuInterface
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(BlueprintCallable)
	void LoadGameMenu();


public: // IMenuInterface
	UFUNCTION(Exec)
	void Host(FString ServerName) override;

	UFUNCTION(Exec)
	void Join(uint32 Index) override;

	void LoadMainMenu() override;
	void RefreshServerList() override;	


private:
	TSubclassOf<class UUserWidget> MenuWidgetClass;
	TSubclassOf<class UUserWidget> GameMenuWidgetClass;

	class UMainMenu* MainMenuWidget;
	class UGameMenu* GameMenuWidget;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	FString DesiredServerName;


private:
	void CreateSession();
	void OnCreateSessionComplete(FName SessionName, bool bSuccess);
	void OnDestroySessionComplete(FName SessionName, bool bSuccess);
	void OnFindSessionsComplete(bool bSuccess);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	

public:
	UMultiplayerMasterGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();
};
