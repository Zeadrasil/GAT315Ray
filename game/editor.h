#pragma once
#include "raylib.h"
#include "body.h"

typedef struct ncEditorData
{
	float massMin;
	float massMax;
	float gravitation;
	int bodyType;
	float damping;
	float gravityScale;
	Color color;
	bool randomColor;
	bool editorBoxActive;
	bool bodyTypeEditMode;
	bool editingBody;
	Vector2 anchor;
	float desiredLengthModifier;
	float stiffness;
	float restitution;
	float physicsFrames;
	bool deleteThings;
	bool playing;
}ncEditorData_t;

extern ncEditorData_t editorData;
extern bool ncEditorActive;
extern bool ncEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 mousePosition);
void DrawEditor(Vector2 position);
struct ncBody* GetBodyIntersect(struct ncBody* bodies, Vector2 position);
void DrawLineBodyToPosition(struct ncBody* body, Vector2 position);