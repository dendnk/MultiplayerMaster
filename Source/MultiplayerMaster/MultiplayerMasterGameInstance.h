#pragma once

#include "Engine/GameInstance.h"
#include "MultiplayerMasterGameInstance.generated.h"


UCLASS()
class MULTIPLAYERMASTER_API UMultiplayerMasterGameInstance 
	: public UGameInstance
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


public:
	UMultiplayerMasterGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();
};
