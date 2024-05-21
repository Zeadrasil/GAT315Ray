#include "world.h"
#include "mathf.h"
#include "world.h"
#include <assert.h>
ncBody* ncBodies = NULL;
int ncBodyCount = 0;
Vector2 ncGravity = { 0, 19.6f };

ncBody* CreateBody(Vector2 position, float mass, ncBodyType bodyType)
{
	ncBody* body = (ncBody*)malloc(sizeof(ncBody));
	assert(body);
	memset(body, 0, sizeof(ncBody));
	body->position = position;
	body->mass = mass;
	body->inverseMass = (bodyType == BT_DYNAMIC) ? 1 / mass : 0;
	body->type = bodyType;
	body->restitution = 0.8f;
	AddBody(body);
	return body;
}

ncBody* CreateBodyAdvanced(Vector2 position, float mass, ncBodyType bodyType, float damping, Color color, float gravityScale)
{
	ncBody* body = CreateBody(position, mass, bodyType);
	body->damping = damping;
	body->color = color;
	body->gravityScale = gravityScale;
	return body;
}

void AddBody(ncBody* body)
{
	assert(body);
	body->next = ncBodies;
	body->previous = NULL;
	if (ncBodies)
	{
		ncBodies->previous = body;
	}
	ncBodies = body;
	ncBodyCount++;
}
void DestroyBody(ncBody* body)
{
	assert(body != NULL);
	if(body->previous)
	{
		body->previous->next = body->next;
	}
	if (body->next)
	{
		body->next->previous = body->previous;
	}
	if (body == ncBodies)
	{
		ncBodies = body->next;
	}
	free(body);
	ncBodyCount--;
}

void ApplyGravitation(ncBody* bodies, float strength)
{
	for (ncBody* body1 = bodies; body1; body1 = body1->next)
	{
		for (ncBody* body2 = body1->next; body2; body2 = body2->next)
		{
			Vector2 direction = Vector2Subtract(body1->position, body2->position);
			float distance = Vector2Length(direction);
			distance = Max(distance, 1);
			float force = (body1->mass * body2->mass) / (distance * distance) * strength;
			direction = Vector2Scale(Vector2Normalize(direction), force);
			ApplyForce(body1, Vector2Scale(direction, -1), FM_FORCE);
			ApplyForce(body2, direction, FM_FORCE);
		}
	}
}		