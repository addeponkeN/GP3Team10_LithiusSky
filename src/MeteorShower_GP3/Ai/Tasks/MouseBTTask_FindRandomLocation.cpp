// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseBTTask_FindRandomLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MeteorShower_GP3/MeteorShower_GP3.h"

UMouseBTTask_FindRandomLocation::UMouseBTTask_FindRandomLocation()
{
	NodeName = TEXT("Walk To Random Location");

	//	this task only accepts vectors
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UMouseBTTask_FindRandomLocation, BlackboardKey));
}

EBTNodeResult::Type UMouseBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FNavLocation NavLocation{};

	//	get ai stuff
	AAIController* AiController{OwnerComp.GetAIOwner()};
	const APawn* AIPawn{AiController->GetPawn()};

	//	set origin of the search radius
	const FVector Origin{AIPawn->GetActorLocation()};

	//	get navsys
	const UNavigationSystemV1* NavSys{UNavigationSystemV1::GetCurrent(GetWorld())};

	if(IsValid(NavSys) && NavSys->GetRandomPointInNavigableRadius(Origin, WalkRadius, NavLocation))
	{
		AiController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, NavLocation.Location);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	
	return EBTNodeResult::Succeeded;
}

FString UMouseBTTask_FindRandomLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}
