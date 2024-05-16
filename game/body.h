#pragma once
#include "raylib.h"
#include "raymath.h"
#include "mathf.h"
typedef enum
{
	BT_STATIC,
	BT_KINEMATIC,
	BT_DYNAMIC
}ncBodyType;

typedef enum
{
	FM_FORCE,
	FM_IMPULSE,
	FM_VELOCITY
}ncForceMode;

typedef struct ncBody
{
	ncBodyType type;
	struct ncBody* previous;
	struct ncBody* next;
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 force;

	Color color;

	float inverseMass;
	float age;
	float mass;
	float gravityScale;
	float damping;
	float restitution;
} ncBody;

inline void ApplyForce(ncBody* body, Vector2 force, ncForceMode forceMode)
{
	if (body->type == BT_DYNAMIC)
	{
		switch (forceMode)
		{
		case FM_FORCE:
			body->force = Vector2Add(body->force, force);
			break;
		case FM_IMPULSE:
			body->velocity = Vector2Add(Vector2Scale(force, body->inverseMass), body->velocity);
			break;
		case FM_VELOCITY:
			body->velocity = force;
			break;
		}
	}
}

inline void ClearForce(ncBody* body)
{
	body->force = CreateVector2(0, 0);
}

void Step(ncBody* body, float timestep);