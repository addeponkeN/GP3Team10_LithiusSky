
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ui/MenuBase.h"
#include "MenuManager.generated.h"

USTRUCT(BlueprintType)
struct FMenuItem
{
	GENERATED_BODY()
	FMenuItem() = default;

	FMenuItem(UMenuBase* Item, const TSubclassOf<UMenuBase>& Class)
		: Item(Item),
		  Class(Class)
	{
	}

	UPROPERTY(BlueprintReadWrite)
	UMenuBase* Item;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMenuBase> Class;
	
};

UCLASS()
class METEORSHOWER_GP3_API UMenuManager : public UObject
{
	GENERATED_BODY()
	
protected:
	virtual void PostInitProperties() override;

public:
	UPROPERTY()
	UMenuBase* CurrentMenu;

	UFUNCTION(BlueprintCallable)
	void SetMenu(TSubclassOf<UMenuBase> MenuClass);

	UFUNCTION(BlueprintCallable)
	void OpenMenu(TSubclassOf<UMenuBase> MenuClass);
	
	UFUNCTION(BlueprintCallable)
	void GoBack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMenuItem> Menus;

	UPROPERTY(BlueprintReadWrite)
	TMap<TSubclassOf<UMenuBase>, FMenuItem> MenuMap;

private:

	UMenuBase* GetMenu(TSubclassOf<UMenuBase> MenuClass);
	
	void SetMenuHard(UMenuBase* Menu);
};
