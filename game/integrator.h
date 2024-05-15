#pragma once
#include "body.h"


void ExplicitEuler(ncBody* body, float timeStep)
{
	body->position = Vector2Add(body->position, Vector2Scale(body->velocity, timeStep));
	body->velocity = Vector2Add(body->velocity, Vector2Scale(body->acceleration, timeStep));
}
void SemiImplicitEuler(ncBody* body, float timeStep)
{
	body->velocity = Vector2Add(body->velocity, Vector2Scale(body->acceleration, timeStep));
	body->position = Vector2Add(body->position, Vector2Scale(body->velocity, timeStep));
}