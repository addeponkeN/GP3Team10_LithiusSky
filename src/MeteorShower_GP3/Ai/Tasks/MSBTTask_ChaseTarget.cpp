// Fill out your copyright notice in the Description page of Project Settings.


#include "MSBTTask_ChaseTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UMSBTTask_ChaseTarget::UMSBTTask_ChaseTarget() { }

EBTNodeResult::Type UMSBTTask_ChaseTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
		UObject* TargetValue = Blackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		AActor* TargetActor = Cast<AActor>(TargetValue);
	}
	return EBTNodeResult::Succeeded;
}

FString UMSBTTask_ChaseTarget::GetStaticDescription() const
{
	return FString::Printf(TEXT("Target"));
}

