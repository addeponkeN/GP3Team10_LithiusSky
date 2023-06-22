#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "MeteorShower_GP3/Ui/Widgets/ScreenFader.h"

#include "GameOverHud.generated.h"

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API UGameOverHud : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	UScreenFader* GetFaderBlack() const { return FaderBlack; };
	UFUNCTION(BlueprintCallable)
	UScreenFader* GetFaderWhite() const { return FaderWhite; };

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UScreenFader* FaderBlack;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UScreenFader* FaderWhite;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UCanvasPanel* BasePanel;
};
