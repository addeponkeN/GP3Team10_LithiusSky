// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MSBTTask_ChaseTarget.generated.h"

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API UMSBTTask_ChaseTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UMSBTTask_ChaseTarget();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY()
	AActor* Target;
	
};
