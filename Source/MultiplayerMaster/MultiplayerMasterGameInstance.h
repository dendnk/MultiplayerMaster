#pragma once

#include "Engine/GameInstance.h"
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
	void Host() override;

	UFUNCTION(Exec)
	void Join(const FString& InIPAddress) override;

	void LoadMainMenu() override;	


private:
	TSubclassOf<class UBaseMenuWidget> MenuWidgetClass;
	TSubclassOf<class UBaseMenuWidget> GameMenuWidgetClass;

	class UMainMenu* MainMenuWidget;
	class UGameMenu* GameMenuWidget;


public:
	UMultiplayerMasterGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();
};
