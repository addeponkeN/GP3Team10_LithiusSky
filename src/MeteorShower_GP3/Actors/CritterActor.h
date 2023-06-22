
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"

#include "CritterActor.generated.h"

UCLASS()
class METEORSHOWER_GP3_API ACritterActor : public ACharacter
{
	GENERATED_BODY()
	
public:	
	ACritterActor();

protected:
	virtual void BeginPlay() override;

public:
	void AttackForward();
	void AttackTowards(FVector TargetLocation);

	float Speed{1.f};
};
