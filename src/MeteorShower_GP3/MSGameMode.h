// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "MeteorShower_GP3Character.h"
#include "GameFramework/GameModeBase.h"
#include "GameSubState/GameSubStateManager.h"
#include "MeteorShower_GP3/MeteorTimeSystem/MeteorTimer.h"
#include "Ui/DebugUi.h"
#include "Ui/GameMenuUi.h"
#include "Ui/GameOverHud.h"
#include "Ui/SettingsMenu.h"

#include "MSGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePaused, bool, IsPaused);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameSetCutsceneMode, bool, IsCutscene);

class ACutsceneManager;
class ALoopManager;
class UGameSettingsDataAsset;

UCLASS(minimalapi)
class AMSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMSGameMode();

	UFUNCTION(BlueprintPure, BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static AMSGameMode* GetGameMode(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static AMeteorTimer* GetMeteorTimerStatic(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static AMSGameMode* GetGameModeBad(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static AMeteorShower_GP3Character* GetCharacter(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static void SetCutsceneMode(const UObject* WorldContextObject, bool bIsPlayCutscene);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static void SetCharacterInputEnabled(UObject* WorldContextObject, bool IsEnabled);

protected:
	virtual void StartPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, Category="Pause")
	FOnGamePaused OnGamePaused;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, Category="Cutscene")
	FOnGameSetCutsceneMode OnGameSetCutsceneMode;

	UPROPERTY(EditAnywhere)
	UInputMappingContext* UiMappingContext;

	UPROPERTY(EditAnywhere)
	UInputAction* PauseAction;

	UPROPERTY(EditAnywhere)
	UInputAction* DebugMenuAction;

	//	dont look
	UPROPERTY(BlueprintReadWrite)
	UAudioComponent* CurrentDialogue;

	UFUNCTION(BlueprintCallable)
	void InitMenus();

	UFUNCTION(BlueprintCallable)
	void ToggleDebugMenu();

	UFUNCTION(BlueprintCallable)
	void TogglePause();

	UFUNCTION(BlueprintCallable)
	void PauseGame();

	UFUNCTION(BlueprintCallable)
	void UnpauseGame();

	UFUNCTION(BlueprintCallable)
	void SetShowSettingsMenu(bool bShow);

	UFUNCTION(BlueprintCallable)
	void SetShowCharacterGameUi(bool bShow);

	UFUNCTION()
	void SetCharacterInputEnabled_CPP(bool IsEnabled);

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> DefaultPlayerClass;

	UPROPERTY(BlueprintReadOnly)
	AGameSubStateManager* StateManager;

	UPROPERTY(BlueprintReadOnly)
	ACutsceneManager* CutsceneManager;

	UFUNCTION(BlueprintCallable)
	AMeteorTimer* GetMeteorTimer();

	UFUNCTION(BlueprintCallable)
	ALoopManager* GetLoopManager();

	UFUNCTION(BlueprintCallable)
	float GetHighScoreTimer() const;

	UFUNCTION(BlueprintCallable)
	void ResetHighScoreTimer() { HighScoreTimer = 0.f; };

	UFUNCTION(BlueprintCallable)
	void ResumeHighScoreTimer() { bScoreTimer = true; }

	UFUNCTION(BlueprintCallable)
	void PauseHighScoreTimer() { bScoreTimer = false; };

	UFUNCTION(BlueprintCallable)
	void SetGameStateLanding();

	UFUNCTION(BlueprintCallable)
	void SetGameStateVictory();

	UPROPERTY(BlueprintReadOnly)
	UGameOverHud* GameOverHud;

	UPROPERTY(BlueprintReadOnly)
	UGameMenuUi* GameMenuUi;

	UPROPERTY(BlueprintReadOnly)
	USettingsMenu* SettingsMenuUi;

	UPROPERTY(BlueprintReadOnly)
	UDebugUi* GameDebugMenuUi;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameOverHud> GameOverHudTemplate;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameMenuUi> GameMenuUiTemplate;

	UPROPERTY(BlueprintReadOnly)
	bool bCutsceneMode{false};
	
private:
	UPROPERTY(VisibleAnywhere)
	AMeteorTimer* MeteorTimer;

	UPROPERTY()
	ALoopManager* LoopManager;

	float HighScoreTimer;
	bool bScoreTimer;

	TSubclassOf<UDebugUi> GameDebugUiClass;
	TSubclassOf<UGameOverHud> GameOverHudClass;
	TSubclassOf<UGameMenuUi> GameMenuUiClass;
	TSubclassOf<USettingsMenu> SettingsMenuUiClass;
	TSubclassOf<AMeteorTimer> MeteorTimerClass;

	template <class T>
	TSubclassOf<T> GetGameUiClass(const FString& ClassName);
};

template <class T>
TSubclassOf<T> AMSGameMode::GetGameUiClass(const FString& ClassName)
{
	const FString BlueprintPath = FString::Printf(TEXT("/Game/Blueprints/Ui/%s.%s_C"), *ClassName, *ClassName);
	static ConstructorHelpers::FClassFinder<T> GetDebugUi(*BlueprintPath);
	if (GetDebugUi.Succeeded())
	{
		return GetDebugUi.Class;
	}
	LOGF("failed to load UI: %s", *ClassName);
	return nullptr;
}
