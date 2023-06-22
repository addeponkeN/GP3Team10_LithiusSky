#pragma once

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Poolable.generated.h"

UCLASS()
class METEORSHOWER_GP3_API APoolable : public AActor
{
	GENERATED_BODY()

public:
	APoolable();

protected:
	bool bIsActive = true;

public:
	virtual void Spawned();
	virtual void Returned();
	virtual void Kill();

	void OnCreated(const FName& folderPath);

private:
	void SetActive(bool isActive);
};
