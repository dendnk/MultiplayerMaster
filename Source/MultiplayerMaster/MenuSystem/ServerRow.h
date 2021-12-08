#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ServerRow.generated.h"

/**
 *  Server Row widget used for displaying sessions that was founded in join menu
 */
UCLASS()
class MULTIPLAYERMASTER_API UServerRow
	: public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* ServerName;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* HostUsername;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* ConnectionFraction;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* Ping;

	UPROPERTY(BlueprintReadOnly)
	bool bSelected = false;

	UPROPERTY(EditAnywhere)
	bool bIsHeader = false;

	UPROPERTY(EditAnywhere)
	FLinearColor SelectedAndHoveredButtonColor = FLinearColor::Blue;

	UPROPERTY(EditAnywhere)
	FLinearColor SelectedAndNonHoveredButtonColor = FLinearColor::Green;

	UPROPERTY(EditAnywhere)
	FLinearColor NonSelectedAndHoveredButtonColor = FLinearColor::Yellow;

	UPROPERTY(EditAnywhere)
	FLinearColor NonSelectedAndNonHoveredButtonColor = FLinearColor::White;


private:
	UPROPERTY(Meta = (BindWidget))
	class UButton* RowButton;

	UPROPERTY()
	class UMainMenu* Parent;


private:
	uint32 Index;


private:
	UFUNCTION()
	void OnClicked();


public:
	void Setup(class UMainMenu* Parent, uint32 Index);


protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
};
