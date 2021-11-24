#pragma once

#include "Engine/GameInstance.h"
#include "MultiplayerMasterGameInstance.generated.h"


UCLASS()
class MULTIPLAYERMASTER_API UMultiplayerMasterGameInstance 
	: public UGameInstance
{
	GENERATED_BODY()
	

public:
	UFUNCTION(Exec)
	void Host();

	UFUNCTION(Exec)
	void Join(const FString& InIPAddress);


public:
	UMultiplayerMasterGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();
};
