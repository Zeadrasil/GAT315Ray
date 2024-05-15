#pragma once
#include "raylib.h"

typedef struct ncContact
{
	struct ncBody* body1;
	struct ncBody* body2;

	float restitution;
	float depth;
	Vector2 normal;

	struct ncContact* next;
} ncContact;

void AddContact(ncContact* contact, ncContact** contacts);
void DestroyAllContacts(ncContact** contacts);
