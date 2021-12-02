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

void UMainMenu::SetServerList(const TArray<FServerData>& ServerDatas)
{
	auto World = GetWorld();
	if (!World)
		return;

	ServerList->ClearChildren();
	SessionsWaitWidget->SetVisibility(ESlateVisibility::Hidden);

	uint32 Index = 0;
	for (const auto& ServerData : ServerDatas)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!Row)
			return;

		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		Row->HostUsername->SetText(FText::FromString(ServerData.HostUsername));
		FString FractionString = FString::Printf(TEXT("%d / %d"),ServerData.CurrentPlayers, ServerData.MaxPlayers); 
		Row->ConnectionFraction->SetText(FText::FromString(FractionString));
		
		Row->Setup(this, Index++);
		
		ServerList->AddChild(Row);	
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		if (const auto ServerRow = Cast<UServerRow>(ServerList->GetChildAt(i)))
		{
			ServerRow->bSelected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
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

	ServerList->ClearChildren();
	SessionsWaitWidget->SetVisibility(ESlateVisibility::Visible);
	
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
