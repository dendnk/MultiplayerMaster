#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "ServerRow.h"


UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ServerRowBPClass.Class)
		return;

	ServerRowClass = ServerRowBPClass.Class;
}

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

void UMainMenu::SetServerList(const TArray<FString>& ServerNames)
{
	auto World = GetWorld();
	if (!World)
		return;

	ServerList->ClearChildren();

	uint32 Index = 0;
	for (const auto& ServerName : ServerNames)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!Row)
			return;

		Row->ServerName->SetText(FText::FromString(ServerName));
		Row->Setup(this, Index++);
		
		ServerList->AddChild(Row);	
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && MenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index : %d"), SelectedIndex.GetValue());
		MenuInterface->Join( SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index not set"));
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!MenuSwitcher || !JoinMenuWidget)
		return;

	MenuSwitcher->SetActiveWidget(JoinMenuWidget);
	
	if (MenuInterface != nullptr)
	{
		MenuInterface->RefreshServerList();
	}
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
