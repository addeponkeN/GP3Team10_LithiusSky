
#include "GigaCheckpoint.h"

AGigaCheckpoint::AGigaCheckpoint()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGigaCheckpoint::BeginPlay()
{
	Super::BeginPlay();
}

void AGigaCheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

