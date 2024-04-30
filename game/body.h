#pragma once
#include "raylib.h"

typedef struct Body
{
	struct Body* previous;
	struct Body* next;
	Vector2 position;
	Vector2 velocity;
	Vector2 force;
	float age;
} Body;