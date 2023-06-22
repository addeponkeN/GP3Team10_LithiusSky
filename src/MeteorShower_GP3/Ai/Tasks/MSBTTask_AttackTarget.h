
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MSBTTask_AttackTarget.generated.h"

UENUM()
enum EAttackState
{
	Entering UMETA(DisplayName = "Entering"),
	Attacking UMETA(DisplayName = "Attacking"),
	Exiting UMETA(DisplayName = "Exiting"),
};

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API UMSBTTask_AttackTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UMSBTTask_AttackTarget();

	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<EAttackState> State{EAttackState::Entering};

	UPROPERTY(EditAnywhere, Category=AttackTarget)
	float AttackTime{1.0f};
	
	UPROPERTY(EditAnywhere, Category=AttackTarget)
	float EnterTime{0.25f};
	
	UPROPERTY(EditAnywhere, Category=AttackTarget)
	float ExitTime{0.5f};

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	void NextState(UBehaviorTreeComponent& OwnerComp, uint8* NodeMem);

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
