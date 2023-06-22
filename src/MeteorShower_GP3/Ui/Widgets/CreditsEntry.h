
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CreditsEntry.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API UCreditsEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* NameText;

	void SetText(const FText& Value) const;
};
