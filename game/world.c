#include "world.h"
#include "mathf.h"
#include <assert.h>
Body* bodies = NULL;
int bodyCount = 0;

Body* CreateBody()
{
	Body* body = (Body*)malloc(sizeof(Body));
	assert(body != NULL);
	body->next = bodies;
	body->previous = NULL;
	if (bodies)
	{
		bodies->previous = body;
	}
	bodies = body;
	bodyCount++;
	return body;
}
void DestroyBody(Body* body)
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
	if (body == bodies)
	{
		bodies = body->next;
	}
	free(body);
	bodyCount--;
}