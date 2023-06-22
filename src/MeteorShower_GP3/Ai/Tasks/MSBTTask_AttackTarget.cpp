#include "MSBTTask_AttackTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "MeteorShower_GP3/MeteorShower_GP3.h"
#include "MeteorShower_GP3/Actors/CritterActor.h"

UMSBTTask_AttackTarget::UMSBTTask_AttackTarget()
{
	NodeName = "AttackTarget";
	bTickIntervals = true;

	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UMSBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	State = EAttackState::Entering;

	// const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	// UObject* TargetValue = Blackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
	// const AActor* TargetActor = Cast<AActor>(TargetValue);
	//
	// if(!TargetActor)
	// {
	// 	LOGW("no target when attacking");
	// 	return EBTNodeResult::Aborted;
	// }

	SetNextTickTime(NodeMemory, EnterTime);
	return EBTNodeResult::InProgress;
}

void UMSBTTask_AttackTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (State == EAttackState::Exiting)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	if (State == EAttackState::Attacking)
	{
		const AAIController* AiController{OwnerComp.GetAIOwner()};
		APawn* Pawn = AiController->GetPawn();
		ACritterActor* Critter = Cast<ACritterActor>(Pawn);

		const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
		UObject* TargetValue = Blackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		const AActor* TargetActor = Cast<AActor>(TargetValue);

		if (TargetActor)
		{
			Critter->AttackTowards(TargetActor->GetActorLocation());
		}
		else
		{
			LOGW("no target when attacking");
		}
	}

	// LOGF("AttackState: %i", State.GetValue());
	NextState(OwnerComp, NodeMemory);
}

void UMSBTTask_AttackTarget::NextState(UBehaviorTreeComponent& OwnerComp, uint8* NodeMem)
{
	if (State == EAttackState::Entering)
	{
		State = EAttackState::Attacking;
		SetNextTickTime(NodeMem, AttackTime);
	}
	else if (State == EAttackState::Attacking)
	{
		State = EAttackState::Exiting;
		SetNextTickTime(NodeMem, ExitTime);
	}
}

FString UMSBTTask_AttackTarget::GetStaticDescription() const
{
	return FString::Printf(TEXT("AttackTarget: %i"), State.GetValue());
}
