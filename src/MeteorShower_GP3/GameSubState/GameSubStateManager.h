#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeteorShower_GP3/MeteorShower_GP3.h"
#include "States/BaseSubState.h"
#include "GameSubStateManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChanged, ESubGameStates, StateType);

UCLASS()
class METEORSHOWER_GP3_API AGameSubStateManager : public AActor
{
	GENERATED_BODY()

public:
	AGameSubStateManager();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnStateChanged OnStateChanged;
	
	UPROPERTY()
	ABaseSubState* CurrentState;

	UPROPERTY()
	ABaseSubState* NextState;

	UPROPERTY()
	ABaseSubState* DefaultState;

	UFUNCTION()
	void SetState(ABaseSubState* State);

	template <class TState>
	TState* SetState();

	UFUNCTION()
	void ExitCurrent();

	UFUNCTION()
	void CurrentStateExited();

private:
	UPROPERTY()
	TMap<class UObject*, ABaseSubState*> States;
};


template <class TState>
TState* AGameSubStateManager::SetState()
{
	static_assert(std::is_base_of_v<ABaseSubState, TState>, "TState must be derived from ABaseSubState");

	UWorld* World = GetWorld();

	if(!World)
	{
		LOGE("World null on SetState!!")
		return nullptr;
	}
	
	ABaseSubState* StateActor;
	UObject* StateClass = TState::StaticClass();
	
	if (!States.Contains(StateClass))
	{
		StateActor = GetWorld()->SpawnActor<TState>();
		States.Add(StateClass, StateActor);
	}
	else
	{
		StateActor = States[StateClass];
	}

	if(!StateActor)
	{
		LOGE("State null on SetState!!")
		return nullptr;
	}

	SetState(StateActor);
	
	return Cast<TState>(StateActor);
}
