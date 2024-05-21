#include "raylib.h"
#include "body.h"
#include "mathf.h"
#include <stdlib.h>
#include <stdio.h>
#include "raymath.h"
#include "world.h"
#include "integrator.h"
#include "render.h"
#include "editor.h"
#include "spring.h"
#include "collision.h"
#include "contact.h"
#define MAX_BODIES 10000
#define MAX_SPRINGS 10000000
#define MAX_AGE 120
#define CREATION_TYPE 0
int main(void)
{
	ncBody* selectedBody = NULL;
	ncBody* connectBody = NULL;
	InitWindow(2560, 1440, "Physics Engine");
	InitEditor();
	SetTargetFPS(100);
	float fixedTimeDelta = 0;
	float stepSize = 1.0f / 60;
	while (!WindowShouldClose())
	{
		//Update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();
		Vector2 position = GetMousePosition();
		fixedTimeDelta += dt;
		UpdateEditor(position);
		ncScreenZoom += GetMouseWheelMove() * 0.1f;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1, 10);
		selectedBody = GetBodyIntersect(ncBodies, position);
		if (selectedBody)
		{
			Vector2 screen = ConvertWorldToScreen(selectedBody->position);
			DrawCircleLines(screen.x, screen.y, ConvertWorldToPixel(selectedBody->mass) * 1.5f, YELLOW);
		}
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ncBodyCount < MAX_BODIES && !editorData.editingBody)
		{
			Color color = editorData.randomColor ? ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1) : editorData.color;

			switch (CREATION_TYPE)
			{
			default:
			{
				CreateBodyAdvanced(ConvertScreenToWorld(position), GetRandomFloatValue(editorData.massMin, editorData.massMax), editorData.bodyType, editorData.damping, color, editorData.gravityScale);
				break;
			}
			case 1:
			{
				//Pattern A
				for (int i = 0; i < 200; i++)
				{
					ncBody* body = CreateBodyAdvanced(ConvertScreenToWorld(position), GetRandomFloatValue(editorData.massMin, editorData.massMax), editorData.bodyType, editorData.damping, color, editorData.gravityScale);
					ApplyForce(body, Vector2Scale(GetVector2FromAngle(GetRandomFloatValue(0, 360) * DEG2RAD), GetRandomFloatValue(200, 400)), FM_IMPULSE);
				}
				break;
			}
			case 2:
			{
				//Pattern B
				float angle = GetRandomFloatValue(0, 360);
				for (int j = 0; j < 100; j++)
				{
					ncBody* body = CreateBodyAdvanced(ConvertScreenToWorld(position), GetRandomFloatValue(editorData.massMin, editorData.massMax), editorData.bodyType, editorData.damping, color, editorData.gravityScale);
					ApplyForce(body, Vector2Scale(GetVector2FromAngle((angle + GetRandomFloatValue(-20, 20)) * DEG2RAD), GetRandomFloatValue(200, 400)), FM_IMPULSE);
				}
				break;
			}
			case 3:
			{
				//Pattern C
				float baseRotation = GetRandomFloatValue(0, 360);
				for (int k = 0; k < 5; k++)
				{
					for (int l = 0; l < 30; l++)
					{
						ncBody* body = CreateBodyAdvanced(ConvertScreenToWorld(position), GetRandomFloatValue(editorData.massMin, editorData.massMax), editorData.bodyType, editorData.damping, color, editorData.gravityScale);
						ApplyForce(body, Vector2Scale(GetVector2FromAngle(((72 * k) + baseRotation + GetRandomFloatValue(-7.5, 7.5)) * DEG2RAD), GetRandomFloatValue(300, 600)), FM_IMPULSE);
					}
				}
				for (int m = 0; m < 300; m++)
				{
					ncBody* body = CreateBodyAdvanced(ConvertScreenToWorld(position), GetRandomFloatValue(editorData.massMin, editorData.massMax), editorData.bodyType, editorData.damping, color, editorData.gravityScale);
					ApplyForce(body, Vector2Scale(GetVector2FromAngle((GetRandomFloatValue(0, 360) + baseRotation) * DEG2RAD), GetRandomFloatValue(100, 400)), FM_IMPULSE);
				}
				break;
			}
			}
		}
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody)
		{
			connectBody = selectedBody;
		}
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody)
		{
			DrawLineBodyToPosition(connectBody, position);
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody && selectedBody != connectBody)
		{
			ncSpring* spring = CreateSpring(connectBody, selectedBody, Vector2Distance(connectBody->position, selectedBody->position) * editorData.desiredLengthModifier, editorData.stiffness);
		}
		ncBody* body = ncBodies;
		ncContact* contacts = NULL;
		while (fixedTimeDelta > stepSize)
		{
			while (body)
			{
				if (body->age > MAX_AGE)
				{
					ncBody* holder = body;
					body = body->next;
					DeleteSpringsWithBody(holder, ncSprings);
					DestroyBody(holder);
				}
				else
				{
					Step(body, stepSize);
					body = body->next;
				}
			}
			ApplyGravitation(body, editorData.gravitation);
			//spring
			ncSpring* spring = ncSprings;
			while (spring)
			{
				ApplySpringForce(spring);
				spring = spring->next;
			}
			//collision
			CreateContacts(ncBodies, &contacts);
			SeparateContacts(contacts);
			ResolveContacts(contacts);
			fixedTimeDelta -= stepSize;
		}
		//Draw
		BeginDrawing();
		ClearBackground(BLACK);
		DrawEditor(position);
		DrawText(TextFormat("FPS: %.2f", fps), 10, 10, 20, LIME);
		DrawText(TextFormat("Frame: %.4f", dt * 1000), 10, 30, 20, LIME);
		DrawText(TextFormat("Bodies: %i", ncBodyCount), 10, 50, 20, LIME);
		DrawText(TextFormat("Springs: %i", ncSpringCount), 10, 70, 20, LIME);
		int i = 0;
		ncContact* contact = contacts;
		while(contact)
		{
			i++;
			contact = contact->next;
		}
		DrawText(TextFormat("Contacts: %i", i), 10, 90, 20, LIME);
		//DrawCircle((int)position.x, (int)position.y, 20, YELLOW);
		body = ncBodies;
		while (body)
		{
			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(body->mass * 0.5f), body->color);
			Vector2 laterScreen = ConvertWorldToScreen(Vector2Add(body->position, Vector2Scale(body->velocity, 5)));
			DrawLine((int)screen.x, (int)screen.y, (int)laterScreen.x, (int)laterScreen.y, WHITE);
			body = body->next;
		}
		contact = contacts;
		while (contact)
		{
			Vector2 screen = ConvertWorldToScreen(Vector2Scale(Vector2Add(contact->body1->position,contact->body2->position), 0.5f));
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel((contact->body1->mass + contact->body2->mass) * 0.125f), RED);
			contact = contact->next;
		}
		EndDrawing();
	}
	free(ncBodies);
	CloseWindow();
	return 0;
}