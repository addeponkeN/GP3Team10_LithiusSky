#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CutscenePlayer.generated.h"

class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCutsceneEnd);

UCLASS()
class METEORSHOWER_GP3_API UCutscenePlayer : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnCutsceneEnd OnCutsceneEnd;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void EndCutscene();

	UFUNCTION(BlueprintCallable)
	void TriggerOnCutsceneEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UImage* CutsceneImage;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void StopCutscene();
};
