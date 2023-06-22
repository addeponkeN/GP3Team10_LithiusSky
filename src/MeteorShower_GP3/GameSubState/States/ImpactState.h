#pragma once

#include "CoreMinimal.h"
#include "BaseSubState.h"
#include "ImpactState.generated.h"

class AMeteorShower_GP3Character;
class AMSGameMode;
/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API AImpactState : public ABaseSubState
{
	GENERATED_BODY()

public:
	AImpactState();

protected:
	virtual void BeginPlay() override;

public:
	virtual void StartEnterState() override;
	virtual void OnEnteredState() override;
	virtual void StartExitState() override;
	virtual void OnExitedState() override;

private:
	UPROPERTY()
	AMSGameMode* GameMode;

	UPROPERTY()
	AMeteorShower_GP3Character* Character;
};
