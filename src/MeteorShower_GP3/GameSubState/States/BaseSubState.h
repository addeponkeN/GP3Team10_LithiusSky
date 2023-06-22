#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BaseSubState.generated.h"

class AGameSubStateManager;

UENUM()
enum ESubGameStates
{
	None UMETA(DisplayName = "None"),
	Landing UMETA(DisplayName = "Landing"),
	Playing UMETA(DisplayName = "Playing"),
	Dead UMETA(DisplayName = "Dead"),
	Impact UMETA(DisplayName = "Impact"),
	Restart UMETA(DisplayName = "Restart"),
	Victory UMETA(DisplayName = "Victory"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateExitedEvent);

UCLASS()
class METEORSHOWER_GP3_API ABaseSubState : public AActor
{
	GENERATED_BODY()

public:
	ABaseSubState();

	ESubGameStates StateType;

	UPROPERTY()
	AGameSubStateManager* StateManager;

	UPROPERTY()
	FOnStateExitedEvent OnStateExitedEvent;

protected:
	virtual void BeginPlay() override;

	/**
	 * @brief When you have entered the state (after StartState)
	 */
	virtual void OnEnteredState();

	/**
	 * @brief When you exited the state (after StartExitState)
	 */
	virtual void OnExitedState();

public:
	/**
	 * @brief When you first enter the state
	 */
	virtual void StartEnterState();

	/**
	 * @brief When you start exiting the state
	 */
	virtual void StartExitState();

	/**
	 * @brief Time to enter
	 */
	UPROPERTY()
	float EnterTime{0.f};

	/**
	 * @brief Time to exit
	 */
	UPROPERTY()
	float ExitTime{0.f};

	/**
 	* @brief The state has been exited
 	*/
	UPROPERTY()
	bool bExited{false};

	/**
	 * @brief Is currently exiting the state
	 */
	UPROPERTY()
	bool bExiting{false};

private:
	FTimerManager* TimerManager;
	FTimerHandle TimerHandle;
};
