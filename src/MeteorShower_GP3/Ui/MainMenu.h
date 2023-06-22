#pragma once

#include "CoreMinimal.h"
#include "MenuBase.h"
#include "MainMenu.generated.h"

UCLASS()
class METEORSHOWER_GP3_API UMainMenu : public UMenuBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
};
