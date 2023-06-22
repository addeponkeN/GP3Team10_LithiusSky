#pragma once

#include "CoreMinimal.h"
#include "MeteorShower_GP3Character.h"
#include "GameFramework/Actor.h"
#include "CutsceneManager.generated.h"

class UCutsceneManagerDataAsset;
class UCutscenePlayer;

UCLASS()
class METEORSHOWER_GP3_API ACutsceneManager : public AActor
{
	GENERATED_BODY()

public:
	ACutsceneManager();

protected:
	virtual void BeginPlay() override;
	void AttachToCharacter(AMeteorShower_GP3Character* Target);

public:
	UPROPERTY(EditAnywhere)
	UCutsceneManagerDataAsset* Settings;

	//returns true if playing cutscene
	UFUNCTION(BlueprintCallable)
	bool PlayCutscene(TSubclassOf<UUserWidget> CutsceneWidgetClass);

	UFUNCTION(BlueprintCallable)
	void EndCutscene();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnEndCutscene();

	UFUNCTION(BlueprintCallable)
	void SkipCutscene();

	UPROPERTY(BlueprintReadWrite)
	UCutscenePlayer* CurrentCutscene;
};
