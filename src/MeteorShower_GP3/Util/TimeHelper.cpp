#include "TimeHelper.h"

FString UTimeHelper::FormatToTime(float Time)
{
	const int32 Minutes = static_cast<int32>(Time) / 60;
	const float Seconds = static_cast<int32>(Time) % 60;
	const float Milliseconds = (Time - FMath::FloorToInt(Time)) * 100;

	FString FormattedTime;
	if (Minutes > 0)
	{
		FormattedTime = FString::Printf(TEXT("%d:%02.0f.%02.0f"), Minutes, Seconds, Milliseconds);
	}
	else if (Seconds < 10)
	{
		FormattedTime = FString::Printf(TEXT("%.0f.%02.0f"), Seconds, Milliseconds);
	}
	else
	{
		FormattedTime = FString::Printf(TEXT("%02.0f.%02.0f"), Seconds, Milliseconds);
	}

	return FormattedTime;
}

FString UTimeHelper::FormatToTimeFull(float Time)
{
	const int32 Minutes = static_cast<int32>(Time) / 60;
	const float Seconds = static_cast<int32>(Time) % 60;
	const float Milliseconds = (Time - FMath::FloorToInt(Time)) * 100;

	// FString FormattedTime;
	return FString::Printf(TEXT("%d:%02.0f.%02.0f"), Minutes, Seconds, Milliseconds);
	// return FormattedTime;
}
