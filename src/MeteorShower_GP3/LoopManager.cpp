#include "LoopManager.h"

ALoopManager::ALoopManager()
{
}

void ALoopManager::RestartLoop()
{
	LoopCount++;
	OnLoopRestarted.Broadcast();
}

void ALoopManager::ResetCounter()
{
	LoopCount = 0;
}
