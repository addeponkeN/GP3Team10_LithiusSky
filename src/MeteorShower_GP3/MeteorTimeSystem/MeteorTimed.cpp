
#include "MeteorTimed.h"

// Add default functionality here for any IMeteorTimed functions that are not pure virtual.
void IMeteorTimed::Register(AMeteorTimer* MeteorTimer)
{
	MeteorTimer->OnTimerChangedEvent.AddDynamic(this, &IMeteorTimed::OnSetTime);
}
