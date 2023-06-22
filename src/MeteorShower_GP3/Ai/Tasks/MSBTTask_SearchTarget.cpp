// Fill out your copyright notice in the Description page of Project Settings.


#include "MSBTTask_SearchTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MeteorShower_GP3/MeteorShower_GP3.h"
#include "MeteorShower_GP3/MeteorShower_GP3Character.h"

UMSBTTask_SearchTarget::UMSBTTask_SearchTarget()
{
	NodeName = "Search Target";
	SearchTime = 3.f;
	RandomDeviation = 1.f;
	bTickIntervals = true;

	//	accept only actors
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UMSBTTask_SearchTarget, BlackboardKey),
	                              AActor::StaticClass());

	//	VERY IMPORTANT IF U WANNA TickTask()
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UMSBTTask_SearchTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AiController{OwnerComp.GetAIOwner()};

	AiController->GetBlackboardComponent()->SetValueAsObject(BlackboardKey.SelectedKeyName, nullptr);

	FinalSearchTime = SearchTime + FMath::FRandRange(0.f, RandomDeviation);
	SetNextTickTime(NodeMemory, SearchTickRate);
	return EBTNodeResult::InProgress;
}

void UMSBTTask_SearchTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FinalSearchTime -= SearchTickRate;
	AAIController* AiController{OwnerComp.GetAIOwner()};
	AMeteorShower_GP3Character* Character = Cast<AMeteorShower_GP3Character>(
		GetWorld()->GetFirstPlayerController()->GetCharacter());

	const float DistanceToTarget = Character->GetDistanceTo(AiController->GetPawn());

	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;

	if (DistanceToTarget < SearchRadius)
	{
		AiController->GetBlackboardComponent()->SetValueAsObject(BlackboardKey.SelectedKeyName, Character);
		NodeResult = EBTNodeResult::Succeeded;
	}
	else if (FinalSearchTime <= 0.f)
	{
		NodeResult = EBTNodeResult::Failed;
	}

	if (NodeResult != EBTNodeResult::InProgress)
	{
		// LOG("search done");
		FinishLatentTask(OwnerComp, NodeResult);
	}
	else
	{
		// LOG("searching...");
		SetNextTickTime(NodeMemory, SearchTickRate);
	}
}

void UMSBTTask_SearchTarget::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                   EBTDescriptionVerbosity::Type Verbosity,
                                                   TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
	FBTTaskMemory* TaskMem = GetSpecialNodeMemory<FBTTaskMemory>(NodeMemory);
	if (TaskMem->NextTickRemainingTime)
	{
		Values.Add(FString::Printf(TEXT("remaining: %ss"), *FString::SanitizeFloat(TaskMem->NextTickRemainingTime)));
	}
}

FString UMSBTTask_SearchTarget::GetStaticDescription() const
{
	return FString::Printf(TEXT("SearchTime: %.1f+-(%.1fs)"), SearchTime, RandomDeviation);
}
