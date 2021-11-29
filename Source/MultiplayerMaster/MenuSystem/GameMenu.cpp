#include "GameMenu.h"
#include "Components/Button.h"


bool UGameMenu::Initialize()
{
	bool bIsSuccess = Super::Initialize();
	if (!bIsSuccess ||
		!ReturnToGameButton || !MainMenuButton)
		return false;

	ReturnToGameButton->OnClicked.AddDynamic(this, &UGameMenu::ReturnToGame);
	MainMenuButton->OnClicked.AddDynamic(this, &UGameMenu::GoToMainMenu);

	return true;
}

void UGameMenu::ReturnToGame()
{
	Hide();
}

void UGameMenu::GoToMainMenu()
{
	if (MenuInterface)
	{
		Hide();
		MenuInterface->LoadMainMenu();
	}
}
