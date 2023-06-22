#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuBase.generated.h"


class UMenuManager;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitedMenu);

UCLASS()
class METEORSHOWER_GP3_API UMenuBase : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UMenuManager* Manager;
	
public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnExitedMenu OnExitedMenu;

	UPROPERTY(BlueprintReadOnly)
	UMenuBase* Parent;

	UFUNCTION(BlueprintCallable)
	virtual void ShowMenu();
	
	UFUNCTION(BlueprintCallable)
	virtual void HideMenu();

	UFUNCTION(BlueprintCallable)
	void MenuBack();
	
	void ClearParent();
};
