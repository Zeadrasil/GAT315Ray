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
	for (ncSpring* spring = ncSprings; spring; spring = spring->next)
	{
		Vector2 direction = Vector2Subtract(spring->left->position, spring->right->position);
		if (direction.x == 0 && direction.y == 0) continue;

		float length = Vector2Length(direction);
		float x = spring->desiredLength - length;
		float force = x * spring->stiffness;

		Vector2 ndirection = Vector2Normalize(direction);

		ApplyForce(spring->left, Vector2Scale(ndirection, force), FM_FORCE);
		ApplyForce(spring->right, Vector2Scale(ndirection, -force), FM_FORCE);
	}
}

void DeleteSpringsWithBody(ncBody* body, ncSpring* springs)
{
	ncSpring* previous;
	for(ncSpring* spring = springs; spring; spring = spring->next)
	{
		if (body == spring->left || body == spring->right)
		{
			previous->next = spring->next;
		}
		else
		{
			previous = spring;
		}
	}
}