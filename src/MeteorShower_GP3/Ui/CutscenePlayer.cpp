
#include "CutscenePlayer.h"

void UCutscenePlayer::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCutscenePlayer::TriggerOnCutsceneEnd()
{
	OnCutsceneEnd.Broadcast();
}

// void UCutscenePlayer::StopCutscene()
// {
	// OnCutsceneEnd.Broadcast();
// }
