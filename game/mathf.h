#pragma once
#include <stdlib.h>
#include "raylib.h"

inline float GetRandomFloatValue01()
{
	return rand() / (float)RAND_MAX;
}
inline float GetRandomFloatValue(float min, float max)
{
	return min + (max - min) * GetRandomFloatValue01();
}

inline Vector2 CreateVector2(float x, float y)
{
	return (Vector2){ x, y };
}

inline Vector2 GetVector2FromAngle(float angle)
{
	Vector2 v;
	v.x = cosf(angle);
	v.y = sinf(angle);
	return v;
}

inline float Max(float a, float b)
{
	if (a > b)
	{
		return a;
	}
	return b;
}