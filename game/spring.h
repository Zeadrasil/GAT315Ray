#pragma once

typedef struct ncSpring
{
	struct ncBody* left;
	struct ncBody* right;
	float desiredLength;
	float stiffness;
	struct ncSpring* previous;
	struct ncSpring* next;
}ncSpring;

extern struct ncSpring* ncSprings;
extern int ncSpringCount;

ncSpring* CreateSpring(struct ncBody* left, struct ncBody* right, float desiredLength, float stiffness);
void AddSpring(ncSpring* spring);
void DestroySpring(ncSpring* spring);

void ApplySpringForce(ncSpring* spring);
void DeleteSpringsWithBody(struct ncBody* body);
void ApplySpringForcePosition(struct Vector2 position, struct ncBody* body, float restLength, float k, float damping);