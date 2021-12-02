#include "ServerRow.h"

#include "Components/Button.h"
#include "MainMenu.h"


void UServerRow::Setup(UMainMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	RowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
}

void UServerRow::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (!bIsHeader)
	{
		FLinearColor ButtonColor;
		if (IsHovered())
		{
			if (bSelected)
			{
				ButtonColor = SelectedAndHoveredButtonColor;
			}
			else
			{
				ButtonColor = NonSelectedAndHoveredButtonColor;
			}			
		}
		else
		{
			if (bSelected)
			{
				ButtonColor = SelectedAndNonHoveredButtonColor;
			}
			else
			{
				ButtonColor = NonSelectedAndNonHoveredButtonColor; 
			}
		}

		RowButton->SetColorAndOpacity(ButtonColor);	
	}
}

void UServerRow::OnClicked()
{
	Parent->SelectIndex(Index);
}
