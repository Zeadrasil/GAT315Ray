#include "editor.h"
#include "render.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

ncEditorData_t editorData;

Rectangle editorRect;

Texture2D cursorTexture;
bool bodyTypeEditMode = true;
float MassMinValue = 0.001f;
float MassMaxValue = 20.0f;
float GravitationMinValue = 0;
float GravitationMaxValue = 100;
float DampingMinValue = 0;
float DampingMaxValue = 10;
float GravityScaleMinValue = -10;
float GravityScaleMaxValue = 10;
void InitEditor()
{
    GuiLoadStyle("raygui/styles/cyber/style_cyber.rgs");
    editorData.massMin = 3;
    editorData.massMax = 6;
    editorData.gravitation = 0;
    editorData.bodyType = 2;
    editorData.anchor = (Vector2){ 10, 120 };
    editorData.editorBoxActive = true;
    editorData.bodyTypeEditMode = false;
    editorData.damping = 0.5f;
    editorData.gravityScale = 1;
    editorData.editingBody = false;
    editorData.desiredLengthModifier = 1.5f;
    editorData.stiffness = 1;
    editorData.randomColor = true;

    Image image = LoadImage("resources/elijah.png");
    HideCursor();
    cursorTexture = LoadTextureFromImage(image);
    UnloadImage(image);

    editorRect = (Rectangle){ editorData.anchor.x + 0, editorData.anchor.y + 0,  520, 456 };
}

void UpdateEditor(Vector2 position)
{
    editorData.editingBody = CheckCollisionPointRec(position, editorRect);
}

void DrawEditor(Vector2 position)
{
    //

    if (editorData.bodyTypeEditMode) GuiLock();

    if (editorData.editorBoxActive);
    {
        editorData.editorBoxActive = !GuiWindowBox((Rectangle) { editorData.anchor.x + 0, editorData.anchor.y + 0, 520, 456 }, "SAMPLE TEXT");
        GuiGroupBox((Rectangle) { editorData.anchor.x + 24, editorData.anchor.y + 56, 224, 232 }, "Body");
        GuiSliderBar((Rectangle) { editorData.anchor.x + 104, editorData.anchor.y + 136, 120, 16 }, "Mass Min", NULL, & editorData.massMin, MassMinValue, editorData.massMax);
        GuiSliderBar((Rectangle) { editorData.anchor.x + 104, editorData.anchor.y + 176, 120, 16 }, "Mass Max", NULL, & editorData.massMax, editorData.massMin, MassMaxValue);
        GuiSliderBar((Rectangle) { editorData.anchor.x + 104, editorData.anchor.y + 216, 120, 16 }, "Gravity Scale", NULL, & editorData.gravityScale, GravityScaleMinValue, GravitationMaxValue);
        GuiSliderBar((Rectangle) { editorData.anchor.x + 104, editorData.anchor.y + 256, 120, 16 }, "Damping", NULL, & editorData.damping, DampingMinValue, DampingMaxValue);
        GuiColorPicker((Rectangle) { editorData.anchor.x + 296, editorData.anchor.y + 88, 152, 152 }, "Body Color", & editorData.color);
        GuiGroupBox((Rectangle) { editorData.anchor.x + 272, editorData.anchor.y + 56, 224, 232 }, "Color");
        GuiCheckBox((Rectangle) { editorData.anchor.x + 304, editorData.anchor.y + 248, 24, 24 }, "Random Body Color", & editorData.randomColor);
        GuiSliderBar((Rectangle) { editorData.anchor.x + 88, editorData.anchor.y + 304, 408, 32 }, "Gravitation", NULL, & editorData.gravitation, 0, 100);
        GuiGroupBox((Rectangle) { editorData.anchor.x + 24, editorData.anchor.y + 360, 472, 72 }, "Springs");
        GuiSliderBar((Rectangle) { editorData.anchor.x + 104, editorData.anchor.y + 400, 120, 16 }, "Stiffness", NULL, & editorData.stiffness, 0, 100);
        GuiSliderBar((Rectangle) { editorData.anchor.x + 352, editorData.anchor.y + 400, 120, 16 }, "Desired Length", NULL, & editorData.desiredLengthModifier, 0, 5);
        if (GuiDropdownBox((Rectangle) { editorData.anchor.x + 48, editorData.anchor.y + 80, 168, 32 }, "STATIC;KINEMATIC;DYNAMIC", & editorData.bodyType, editorData.bodyTypeEditMode)) editorData.bodyTypeEditMode = !editorData.bodyTypeEditMode;
        DrawTexture(cursorTexture, position.x - (cursorTexture.width / 2), position.y - (cursorTexture.height / 2), (Color) { 255, 255, 255, 100 });
    }

    GuiUnlock();
}
ncBody* GetBodyIntersect(ncBody* bodies, Vector2 position)
{
    for (ncBody* body = bodies; body; body = body->next)
    {
        Vector2 screen = ConvertWorldToScreen(body->position);
        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass)))
        {
            return body;
        }
    }

    return NULL;
}

void DrawLineBodyToPosition(ncBody* body, Vector2 position)
{
    Vector2 screen = ConvertWorldToScreen(body->position);
    DrawLine((int)screen.x, (int)screen.y, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, YELLOW);
}