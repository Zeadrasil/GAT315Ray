#include "spring.h"
#include "body.h"
#include <assert.h>

ncSpring* ncSprings = NULL;
int ncSpringCount = 0;
ncSpring* CreateSpring(ncBody* left, ncBody* right, float desiredLength, float stiffness)
{
	ncSpring* spring = (ncSpring*)malloc(sizeof(ncSpring));
	assert(spring);
	memset(spring, 0, sizeof(ncSpring));
	spring->left = left;
	spring->right = right;
	spring->desiredLength = desiredLength;
	spring->stiffness = stiffness;
	AddSpring(spring);
	return spring;
}

void AddSpring(ncSpring* spring)
{
	assert(spring);
	spring->next = ncSprings;
	spring->previous = NULL;
	if (ncSprings)
	{
		ncSprings->previous = spring;
	}
	ncSprings = spring;
	ncSpringCount++;
}
void DestroySpring(ncSpring* spring)
{
	assert(spring != NULL);
	if (spring->previous)
	{
		spring->previous->next = spring->next;
	}
	if (spring->next)
	{
		spring->next->previous = spring->previous;
	}
	if (spring == ncSprings)
	{
		ncSprings = ncSprings->next;
	}
	free(spring);
	ncSpringCount--;
}

void ApplySpringForce(ncSpring* spring)
{
	//iterate through all springs
	for (ncSpring* spring = ncSprings; spring; spring = spring->next)
	{
		//difference between positions of spring bodies
		Vector2 direction = Vector2Subtract(spring->left->position, spring->right->position);
		//skip if same position
		if (direction.x == 0 && direction.y == 0) continue;
		//distance between positions
		float length = Vector2Length(direction);
		//distance from optimal distance
		float x = spring->desiredLength - length;
		//hookes law
		float force = x * spring->stiffness;
		//normalized direction
		Vector2 ndirection = Vector2Normalize(direction);
		//gets the force to apply to bodies by scaling direction by the force magnitude and applying damping
		ApplyForce(spring->left, Vector2Scale(ndirection, force + Vector2DotProduct(spring->left->velocity, ndirection) * 0.5f), FM_FORCE);
		ApplyForce(spring->right, Vector2Scale(ndirection, -force - Vector2DotProduct(spring->right->velocity, ndirection) * 0.5f), FM_FORCE);
	}
}

void DeleteSpringsWithBody(ncBody* body)
{
	ncSpring* spring = ncSprings;
	while( spring)
	{
		//if spring uses body
		if (body == spring->left || body == spring->right)
		{
			//remove spring from springs
			ncSpring* holder = spring->next;
			DestroySpring(spring);
			spring = holder;
		}
		else
		{
			spring = spring->next;
		}
	}
}

void ApplySpringForcePosition(Vector2 position, ncBody* body, float restLength, float k, float damping)
{
	//return if no body
	if (!body) return;
	//direction to apply force
	Vector2 direction = Vector2Subtract(position, body->position);
	//return if no direction
	if (direction.x == 0 && direction.y == 0) return;
	//distance between position and body
	float length = Vector2Length(direction);
	//distance from optimal position
	float x = restLength - length;
	//hookes law
	float force = x * k;
	//normalize direction
	Vector2 ndirection = Vector2Normalize(direction);
	//damping to reduce oscillization
	float dampingForce = damping * Vector2DotProduct(body->velocity, ndirection);
	//total force to apply
	float totalForce = force + dampingForce;
	//applies force
	ApplyForce(body, Vector2Scale(ndirection, -totalForce), FM_FORCE);
}