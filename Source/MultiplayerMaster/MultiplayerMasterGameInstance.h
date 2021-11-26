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
	void LoadMenu();

	UFUNCTION(Exec)
	void Host();

	UFUNCTION(Exec)
	void Join(const FString& InIPAddress);


private:
	TSubclassOf<class UUserWidget> MenuWidgetClass;
	class UMainMenu* MainMenuWidget;


public:
	UMultiplayerMasterGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();
};
