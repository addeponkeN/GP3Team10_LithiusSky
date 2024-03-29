// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "MouseAIController.generated.h"

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API AMouseAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMouseAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess=true))
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI", meta=(AllowPrivateAccess=true))
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI", meta=(AllowPrivateAccess=true))
	TObjectPtr<UBlackboardComponent> BlackboardComponent;
};
