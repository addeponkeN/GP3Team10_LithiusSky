#pragma once

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CutsceneManagerDataAsset.generated.h"

class UInputAction;

UCLASS()
class METEORSHOWER_GP3_API UCutsceneManagerDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UInputAction* SkipAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> IntroCutscene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> VictoryCutscene;
};
