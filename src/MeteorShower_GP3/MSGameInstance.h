#pragma once

#include "CoreMinimal.h"
#include "MeteorShower_GP3Character.h"
#include "SaveManagerInterface.h"
#include "CheckpointSystem/CheckpointSave.h"
#include "Engine/GameInstance.h"
#include "MSGameInstance.generated.h"

class UMenuBase;
class USettingsMenu;
class UMainMenu;
/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API UMSGameInstance : public UGameInstance, public ISaveManagerInterface
{
	GENERATED_BODY()

public:
	
	// static UMSGameInstance* Instance;
	
	UFUNCTION(BlueprintPure, BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static UGameSettingsDataAsset* GetGameSettings(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static UMSGameInstance* GetGameInstance(UObject* WorldContextObject);

protected:
	virtual void Init() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGameSettingsDataAsset* GameSettings;
	
	//	dont look
	UPROPERTY(BlueprintReadWrite)
	UAudioComponent* CurrentMusic;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsDead;

	UFUNCTION(BlueprintCallable)
	AMeteorShower_GP3Character* GetCharacter();

	virtual void AddSaveable_Implementation(USaveableComponent* ComponentToAdd);

	UFUNCTION()
	void SaveTo(UCheckpointSave* Save, ACheckpoint* Checkpoint);

	UFUNCTION()
	void LoadFrom(UCheckpointSave* Save);

	UFUNCTION(BlueprintCallable, Category=SaveGame)
	void SaveCheckpointState(ACheckpoint* Checkpoint);

	UFUNCTION(BlueprintCallable, Category=SaveGame)
	void LoadCheckpointState();

	UFUNCTION(BlueprintCallable, Category=SaveGame)
	void SaveGameState(ACheckpoint* Checkpoint);

	/**
	 * @brief Load & Respawn at the GIGA checkpoint
	 */
	UFUNCTION(BlueprintCallable, Category=SaveGame)
	void LoadGameState();

	UFUNCTION(BlueprintCallable, Category=SaveGame)
	void RestartFromCheckpoint();

	UFUNCTION(BlueprintCallable, Category=SaveGame)
	void RestartFromGigaCheckpoint();
	
	void ResetCheckpoints();

private:
	UPROPERTY(VisibleAnywhere)
	TArray<USaveableComponent*> Saveables;

	UPROPERTY(VisibleAnywhere)
	UCheckpointSave* CurrentSaveState;

	UPROPERTY(VisibleAnywhere)
	UCheckpointSave* CurrentGigaSaveState;

	// UPROPERTY()
	// TArray<uint8> SaveBuffer;
	
	//	bad ui stuff
// public:

	// UPROPERTY()
	// AMenuManager* MenuManager;
	
	// void SetCurrentMenu(UMenuBase* Menu);
	//
	// UPROPERTY()
	// UMenuBase* CurrentUi;
	//
	// UPROPERTY(BlueprintReadWrite)
	// UMainMenu* MainMenuUi;
	//
	// UPROPERTY(BlueprintReadWrite)
	// USettingsMenu* SettingsMenuUi;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// TSubclassOf<UMainMenu> MainMenuClass;
};
