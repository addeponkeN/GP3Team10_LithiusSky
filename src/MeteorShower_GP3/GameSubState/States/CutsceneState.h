
#pragma once

#include "CoreMinimal.h"
#include "BaseSubState.h"
#include "CutsceneState.generated.h"

class AMSGameMode;

UCLASS()
class METEORSHOWER_GP3_API ACutsceneState : public ABaseSubState
{
	GENERATED_BODY()
	
public:	
	ACutsceneState();

protected:
	virtual void BeginPlay() override;

	virtual void StartEnterState() override;
	virtual void OnEnteredState() override;
	virtual void StartExitState() override;
	virtual void OnExitedState() override;

private:

	UPROPERTY()
	AMSGameMode* GameMode;

};
