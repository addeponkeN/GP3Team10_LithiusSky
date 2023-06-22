#include "MenuManager.h"

#include "MeteorShower_GP3.h"
#include "Blueprint/UserWidget.h"
#include "Ui/MenuBase.h"

void UMenuManager::PostInitProperties()
{
	UObject::PostInitProperties();
}

void UMenuManager::SetMenu(TSubclassOf<UMenuBase> MenuClass)
{
	UMenuBase* MenuToSet = GetMenu(MenuClass);

	if (!MenuToSet)
	{
		return;
	}

	SetMenuHard(MenuToSet);
}

void UMenuManager::OpenMenu(TSubclassOf<UMenuBase> MenuClass)
{
	UMenuBase* MenuToOpen = GetMenu(MenuClass);

	if (!MenuToOpen)
	{
		return;
	}

	MenuToOpen->Parent = CurrentMenu;

	SetMenuHard(MenuToOpen);
}

void UMenuManager::GoBack()
{
	if (!CurrentMenu || !CurrentMenu->Parent)
	{
		return;
	}

	auto MenuToSet = CurrentMenu->Parent;
	CurrentMenu->ClearParent();

	SetMenuHard(MenuToSet);
}

UMenuBase* UMenuManager::GetMenu(TSubclassOf<UMenuBase> MenuClass)
{
	if (MenuMap.Contains(MenuClass))
	{
		const FMenuItem& MenuCache = MenuMap[MenuClass];
		return MenuCache.Item;
	}
	else
	{
		if (const UWorld* World = GetWorld(); !World)
		{
			LOG("world null");
			return nullptr;
		}

		APlayerController* Controller = GetWorld()->GetFirstPlayerController();
		if (!Controller)
		{
			LOG("controller null - SetMenu()");
			return nullptr;
		}

		UMenuBase* RetMenu = CreateWidget<UMenuBase>(Controller, MenuClass);
		if (!RetMenu)
		{
			LOG("failed to create menu - SetMenu()");
			return nullptr;
		}

		RetMenu->bIsFocusable = true;
		RetMenu->AddToViewport();
		const FMenuItem MenuItem = FMenuItem(RetMenu, MenuClass);
		MenuMap.Add(MenuClass, MenuItem);
		return RetMenu;
	}
}

void UMenuManager::SetMenuHard(UMenuBase* Menu)
{
	if (CurrentMenu)
	{
		CurrentMenu->HideMenu();
	}

	CurrentMenu = Menu;
	CurrentMenu->ShowMenu();
	CurrentMenu->SetKeyboardFocus();
}
