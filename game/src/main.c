#include "raylib.h"
#include "body.h"
#include "mathf.h"
#include <stdlib.h>
#include "raymath.h"
#include "world.h"

#define MAX_BODIES 10
int main(void)
{
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(100);

	while (!WindowShouldClose())
	{
		//Update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();
		Vector2 position = GetMousePosition();
		if (IsMouseButtonPressed(0) && bodyCount < MAX_BODIES)
		{
			Body* body = CreateBody();
			body->position = position;
			body->velocity = CreateVector2(GetRandomFloatValue(-5, 5), GetRandomFloatValue(-5,5));
			body->force = CreateVector2(0, 0);
		}
		Body* body = bodies;
		while (body)
		{
			if (body->age > 10)
			{
				Body* holder = body;
				body = body->next;
				DestroyBody(holder);
			}
			else
			{
				body->age += dt;
				body->position = AddVector2(body->position, MultiplyVector2(body->velocity, dt));
				body->velocity = AddVector2(body->velocity, MultiplyVector2(body->force, dt));
				body = body->next;
			}
		}
		//Draw
		BeginDrawing();
		ClearBackground(BLACK);
		DrawText(TextFormat("FPS: %.2f", fps), 10, 10, 20, LIME);
		DrawText(TextFormat("Frame: %.4f", fps * 1000), 10, 30, 20, LIME);
		DrawText(TextFormat("Bodies: %i", bodyCount), 10, 50, 20, LIME);

		DrawCircle((int)position.x, (int)position.y, 20, YELLOW);
		body = bodies;
		while (body)
		{
			DrawCircle((int)body->position.x, (int)body->position.y, 20, RED);
			body = body->next;
		}
		EndDrawing();
	}
	free(bodies);
	CloseWindow();
	return 0;
}