#pragma once
#include "MathUtil.h"

struct Easings
{
	static float ExpoIn(float const x)
	{
		return x == 0.f ? 0.f : FMathf::Pow(2, 10.f * x - 10.f);
	}

	static float ExpoOut(float const x)
	{
		return x == 1.f ? 1.f : 1.f - FMath::Pow(2, -10.f * x);
	}

	static float CubicIn(float const x)
	{
		return x * x * x;
	}

	static float QuartIn(float const x)
	{
		return x * x * x * x;
	}

	static float QuartOut(float const x)
	{
		return 1.f - FMath::Pow(1.f - x, 4.f);
	}

	static float CubicOut(float const x)
	{
		return 1.f - FMath::Pow(1.f - x, 3.f);
	}

	static float CircOut(float const x)
	{
		return FMath::Sqrt(1.f - FMath::Pow(x - 1.f, 2.f));
	}

	static float BounceOut(float x)
	{
		constexpr float n1 = 7.5625f;
		constexpr float d1 = 2.75f;
		if (x < 1.f / d1)
		{
			return n1 * x * x;
		}
		if (x < 2.f / d1)
		{
			return n1 * (x -= 1.5f / d1) * x + 0.75f;
		}
		if (x < 2.5f / d1)
		{
			return n1 * (x -= 2.25f / d1) * x + 0.9375f;
		}
		return n1 * (x -= 2.625f / d1) * x + 0.984375f;
	}

	static float BackInOut(float x)
	{
		const float c1 = 1.70158f;
		const float c2 = c1 * 1.525f;

		return x < 0.5f
			       ? (FMath::Pow(2.f * x, 2.f) * ((c2 + 1.f) * 2.f * x - c2)) / 2.f
			       : (FMath::Pow(2.f * x - 2.f, 2.f) * ((c2 + 1.f) * (x * 2.f - 2.f) + c2) + 2.f) / 2.f;
	}

	static float BackOut(float x)
	{
		const float c1 = 1.70159f;
		const float c3 = c1 + 1.f;

		return 1.f + c3 * FMath::Pow(x - 1.f, 3.f) + c1 * FMath::Pow(x - 1.f, 2.f);
	}
};
