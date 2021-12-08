#include "BaseMenuWidget.h"


void UBaseMenuWidget::Show()
{
	AddToViewport();

	auto World = GetWorld();
	if (!World)
		return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
		return;

	this->bIsFocusable = true;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(true);
}

void UBaseMenuWidget::Hide()
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

void UBaseMenuWidget::SetMenuInterface(IMenuInterface* InMenuInterface)
{
	MenuInterface = InMenuInterface;
}
