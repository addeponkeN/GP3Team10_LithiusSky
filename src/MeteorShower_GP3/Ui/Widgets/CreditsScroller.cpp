#include "CreditsScroller.h"

#include "CreditsEntry.h"
#include "Blueprint/WidgetTree.h"
#include "Components/VerticalBox.h"

void UCreditsScroller::NativeConstruct()
{
	Super::NativeConstruct();

	AddEntry(TEXT("bong"));
	AddEntry(TEXT("frodo"));
	AddEntry(TEXT("gandalf"));
	AddEntry(TEXT("sam"));
	AddEntry(TEXT("LOEGLAS"));
	AddEntry(TEXT("gimli"));
	AddEntry(TEXT("aragorn"));
	AddEntry(TEXT("sauron"));
	AddEntry(TEXT("mr gollum"));
}

void UCreditsScroller::AddEntry(FString Name)
{
	UCreditsEntry* EntryText = WidgetTree->ConstructWidget<UCreditsEntry>(CreditsEntryClass, FName(Name));
	EntryText->SetText(FText::FromString(Name));
	EntryBox->AddChild(EntryText);
	Entries.Add(EntryText);
}
