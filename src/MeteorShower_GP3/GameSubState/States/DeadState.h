#pragma once

#include "CoreMinimal.h"
#include "BaseSubState.h"
#include "MeteorShower_GP3/MSGameMode.h"
#include "MeteorShower_GP3/MeteorShower_GP3Character.h"
#include "DeadState.generated.h"

UCLASS()
class METEORSHOWER_GP3_API ADeadState : public ABaseSubState
{
	GENERATED_BODY()

public:
	ADeadState();

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

	FTimerHandle WaitTimerHandle;
};
