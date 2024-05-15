#include "body.h"
#include "world.h"

void ExplicitEuler(ncBody* body, float timeStep);
void SemiImplicitEuler(ncBody* body, float timeStep);
void Step(ncBody* body, float timeStep)
{
	body->age += timeStep;
	body->force = Vector2Add(body->force, Vector2Scale(Vector2Scale(ncGravity, body->gravityScale), body->mass));
	body->acceleration = Vector2Scale(body->force, body->inverseMass);

	SemiImplicitEuler(body, timeStep);

	float damping = 1 / (1 + (body->damping * timeStep));
	body->velocity = Vector2Scale(body->velocity, damping);

	ClearForce(body);
}