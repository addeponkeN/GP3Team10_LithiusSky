#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "SlowMoNiagara.generated.h"

/**
 * 
 */
UCLASS()
class METEORSHOWER_GP3_API USlowMoNiagara : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	USlowMoNiagara();
protected:
	virtual void BeginPlay() override;
};
