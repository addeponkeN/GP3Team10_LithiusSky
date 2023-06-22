#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MenuCollectionDataAsset.generated.h"

class UMenuBase;

UCLASS()
class METEORSHOWER_GP3_API UMenuCollectionDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMenuBase> StartMenu;
	
	UPROPERTY(EditAnywhere, Category="Menus")
	TArray<TSubclassOf<UMenuBase>> MenuClasses;
	
};
