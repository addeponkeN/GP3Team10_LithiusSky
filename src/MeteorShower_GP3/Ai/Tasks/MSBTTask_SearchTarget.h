// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MSBTTask_SearchTarget.generated.h"

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API UMSBTTask_SearchTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UMSBTTask_SearchTarget();

	UPROPERTY(EditAnywhere, Category=SearchTarget)
	float SearchRadius{600.f};

	UPROPERTY(EditAnywhere, Category=SearchTarget)
	float SearchTime{3.f};

	UPROPERTY(EditAnywhere, Category=SearchTarget)
	float RandomDeviation{0.f};

	UPROPERTY(EditAnywhere, Category=SearchTarget)
	float SearchTickRate{1.f};

	UPROPERTY()
	float FinalSearchTime; 

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	                                   EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
