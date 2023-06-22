#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SaveableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class METEORSHOWER_GP3_API USaveableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USaveableComponent();

protected:
	virtual void BeginPlay() override;

public:
	void Save();
	
private:
	void RegisterSelf();
};
