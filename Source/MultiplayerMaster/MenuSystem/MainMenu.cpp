#include "MainMenu.h"
#include "Components/Button.h"


bool UMainMenu::Initialize()
{
	bool bIsSuccess = Super::Initialize();
	if (!bIsSuccess ||
		!HostButton ||
		!JoinButton)
		return false;
	
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

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
	UE_LOG(LogTemp, Warning, TEXT("I'm gonna join to the server!"));
}

void UMainMenu::SetMenuInterface(IMenuInterface* InMenuInterface)
{
	MenuInterface = InMenuInterface;
}

void UMainMenu::Show()
{
	AddToViewport();

	auto World = GetWorld();
	if (!World)
		return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
		return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(true);
}

void UMainMenu::Hide()
{
	RemoveFromViewport();

	auto World = GetWorld();
	if (!World)
		return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
		return;

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(false);
}
