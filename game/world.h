#pragma once
#include "body.h"

extern ncBody* ncBodies;
extern int ncBodyCount;
extern Vector2 ncGravity;

ncBody* CreateBody(Vector2 position, float mass, ncBodyType bodyType);
ncBody* CreateBodyAdvanced(Vector2 position, float mass, ncBodyType bodyType, float damping, Color color, float gravityScale);
void AddBody(ncBody* body);
void DestroyBody(ncBody* body);
void ApplyGravitation(ncBody* bodies, float strength);