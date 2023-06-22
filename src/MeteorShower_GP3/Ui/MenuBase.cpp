#include "MenuBase.h"

#include "MeteorShower_GP3/MenuManager.h"

void UMenuBase::ShowMenu()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UMenuBase::HideMenu()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UMenuBase::MenuBack()
{
	Manager->GoBack();
}

void UMenuBase::ClearParent()
{
	Parent = nullptr;
}
