#pragma once

#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"


/**
 * Main Menu Widget
 */
UCLASS()
class MULTIPLAYERMASTER_API UMainMenu 
	: public UUserWidget	
{
	GENERATED_BODY()


private:
	UPROPERTY(Meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* JoinButton;


private:
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();


private:
	IMenuInterface* MenuInterface;


public:
	void SetMenuInterface(IMenuInterface* InMenuInterface);
	void Show();
	void Hide();


protected:
	virtual bool Initialize() override;
};
