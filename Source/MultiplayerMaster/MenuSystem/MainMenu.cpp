#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"


bool UMainMenu::Initialize()
{
	bool bIsSuccess = Super::Initialize();
	if (!bIsSuccess ||
		!HostButton || !JoinButton || !JoinMenuButton || !ReturnToMainMenuButton || !QuitGameButton)
		return false;
	
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);	
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	JoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	ReturnToMainMenuButton->OnClicked.AddDynamic(this, &UMainMenu::ReturnToMainMenu);

	QuitGameButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
	
	return true;
}

void UMainMenu::HostServer()
{
	if (MenuInterface)
	{
		MenuInterface->Host();
	}	
}

void UMainMenu::JoinServer()
{
	if (MenuInterface)
	{
		if (!IPAddressField)
			return;

		const FString& Address = IPAddressField->GetText().ToString();
		MenuInterface->Join(Address);

		UE_LOG(LogTemp, Warning, TEXT("I'm gonna join to the server with ip : %s!"), *Address);
	}	
}

void UMainMenu::OpenJoinMenu()
{
	if (!MenuSwitcher || !JoinMenuWidget)
		return;

	MenuSwitcher->SetActiveWidget(JoinMenuWidget);
}

void UMainMenu::ReturnToMainMenu()
{
	if (!MenuSwitcher || !MainMenuWidget)
		return;

	MenuSwitcher->SetActiveWidget(MainMenuWidget);
}

void UMainMenu::QuitGame()
{
	auto World = GetWorld();
	if (!World)
		return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
		return;

	PlayerController->ConsoleCommand(TEXT("quit"));
}
