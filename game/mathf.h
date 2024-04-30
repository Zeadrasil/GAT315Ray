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

inline Vector2 AddVector2(Vector2 a, Vector2 b)
{
	return CreateVector2(a.x + b.x, a.y + b.y);
}

inline Vector2 MultiplyVector2(Vector2 vector, float multiplyBy)
{
	return CreateVector2(vector.x * multiplyBy, vector.y * multiplyBy);
}