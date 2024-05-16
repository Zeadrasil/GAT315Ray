#include "collision.h"
#include "contact.h"
#include "body.h"
#include "mathf.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

bool Intersects(ncBody* body1, ncBody* body2)
{
	float distance = Vector2Distance(body1->position, body2->position);
	float radius = body1->mass + body2->mass;
	return (distance <= radius);
}

void CreateContacts(ncBody* bodies, ncContact** contacts)
{
	for (ncBody* body1 = bodies; body1; body1 = body1->next)
	{
		for (ncBody* body2 = body1->next; body2; body2 = body2->next)
		{
			if (body1->type != BT_DYNAMIC && body2->type != BT_DYNAMIC) continue;
			if (Intersects(body1, body2))
			{
				ncContact* contact = GenerateContact(body1, body2);
				AddContact(contact, contacts);
			}
		}
	}
}
ncContact* GenerateContact(ncBody* body1, ncBody* body2)
{
	ncContact* contact = (ncContact*)malloc(sizeof(ncContact));
	assert(contact);

	memset(contact, 0, sizeof(ncContact));

	contact->body1 = body1;
	contact->body2 = body2;

	Vector2 direction = Vector2Subtract(body1->position, body2->position);
	float distance = Vector2Length(direction);
	if (distance == 0)
	{
		direction = (Vector2){ GetRandomFloatValue(-0.05f, 0.05f), GetRandomFloatValue(-0.05f, 0.05f) };
	}

	float radius = (body1->mass + body2->mass);

	contact->depth = radius - distance;
	contact->normal = Vector2Normalize(direction);
	contact->restitution = (body1->restitution + body2->restitution) * 0.5f;

	return contact;
}

void SeparateContacts(ncContact* contacts)
{
	//loop through all contacts	
	for (ncContact* contact = contacts; contact; contact = contact->next)
	{
		//get total inverse mass
		float totalInverse = contact->body1->inverseMass + contact->body2->inverseMass;
		//get vector required to separate contact bodies
		Vector2 separation = Vector2Scale(contact->normal, contact->depth / totalInverse);
		//move bodies so that they are no longer inside each other
		contact->body1->position = Vector2Add(contact->body1->position, Vector2Scale(separation, contact->body1->inverseMass));
		contact->body2->position = Vector2Add(contact->body2->position, Vector2Scale(separation, -contact->body2->inverseMass));
	}
}
void ResolveContacts(ncContact* contacts)
{
	//loop thorugh all contacts
	for (ncContact* contact = contacts; contact; contact = contact->next)
	{
		//get relative velocities of contact bodies
		Vector2 relativeVelocity = Vector2Subtract(contact->body1->velocity, contact->body2->velocity);
		//gets the scale of the component of relative velocity that is based on contact normal
		float normalVelocity = Vector2DotProduct(relativeVelocity, contact->normal);
		//if they are already separating, do nothing
		if (normalVelocity > 0) continue;
		//gets total inverse mass
		float totalInverse = contact->body1->inverseMass + contact->body2->inverseMass;
		//gets the magnitude of the force that they will bounce off of each other with
		float magnitude = (1 + contact->restitution) * normalVelocity / totalInverse;
		//gets the force they bounce off of each other with using the magnitude of the force and the contact normal
		Vector2 impulse = Vector2Scale(contact->normal, magnitude);
		//applies force of bounce as an impulse
		ApplyForce(contact->body1, Vector2Scale(impulse, -1), FM_IMPULSE);
		ApplyForce(contact->body2, impulse, FM_IMPULSE);
	}
}