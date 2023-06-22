#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreditsScroller.generated.h"

class UCreditsEntry;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API UCreditsScroller : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UVerticalBox* EntryBox;

	UPROPERTY(EditAnywhere)
	TArray<UWidget*> Entries;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCreditsEntry> CreditsEntryClass;
	
	void AddEntry(FString Name);
};
