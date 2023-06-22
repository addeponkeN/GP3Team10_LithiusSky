
#pragma once

#include "CoreMinimal.h"
#include "Checkpoint.h"
#include "GameFramework/Actor.h"
#include "GigaCheckpoint.generated.h"

UCLASS()
class METEORSHOWER_GP3_API AGigaCheckpoint : public ACheckpoint
{
	GENERATED_BODY()
	
public:	
	AGigaCheckpoint();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
