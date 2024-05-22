#include "editor.h"
#include "render.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"
#define EDITOR_DATA(data) TextFormat("%0.2f", data), &data

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
float RestitutionMinValue = 0;
float RestitutionMaxValue = 1;
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
    editorData.restitution = 0.8;
    editorData.physicsFrames = 50;
    editorData.playing = false;
    editorData.deleteThings = false;

    Image image = LoadImage("resources/elijah.png");
    HideCursor();
    cursorTexture = LoadTextureFromImage(image);
    UnloadImage(image);

    editorRect = (Rectangle){ editorData.anchor.x + 0, editorData.anchor.y + 0,  528, 516 };
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
        editorData.editorBoxActive = !GuiWindowBox((Rectangle) { editorData.anchor.x + 0, editorData.anchor.y + 0, 528, 516 }, "SAMPLE TEXT");
        GuiGroupBox((Rectangle) { editorData.anchor.x + 24, editorData.anchor.y + 56, 232, 232 }, "Body");
        GuiSliderBar((Rectangle) { editorData.anchor.x + 112, editorData.anchor.y + 128, 102, 16 }, "Mass Min", EDITOR_DATA(editorData.massMin), MassMinValue, editorData.massMax);
        GuiSliderBar((Rectangle) { editorData.anchor.x + 112, editorData.anchor.y + 160, 102, 16 }, "Mass Max", EDITOR_DATA(editorData.massMax), editorData.massMin, MassMaxValue);
        GuiSliderBar((Rectangle) { editorData.anchor.x + 112, editorData.anchor.y + 192, 102, 16 }, "Gravity Scale", EDITOR_DATA(editorData.gravityScale), GravityScaleMinValue, GravitationMaxValue);
        GuiSliderBar((Rectangle) { editorData.anchor.x + 112, editorData.anchor.y + 224, 102, 16 }, "Damping", EDITOR_DATA(editorData.damping), DampingMinValue, DampingMaxValue);
        GuiSliderBar((Rectangle) { editorData.anchor.x + 112, editorData.anchor.y + 256, 102, 16 }, "Restitution", EDITOR_DATA(editorData.restitution), RestitutionMinValue, RestitutionMaxValue);
        GuiColorPicker((Rectangle) { editorData.anchor.x + 296, editorData.anchor.y + 88, 152, 152 }, "Body Color", & editorData.color);
        GuiGroupBox((Rectangle) { editorData.anchor.x + 280, editorData.anchor.y + 56, 224, 232 }, "Color");
        GuiCheckBox((Rectangle) { editorData.anchor.x + 312, editorData.anchor.y + 248, 24, 24 }, "Random Body Color", & editorData.randomColor);
        GuiSliderBar((Rectangle) { editorData.anchor.x + 96, editorData.anchor.y + 304, 384, 32 }, "Gravitation", EDITOR_DATA(editorData.gravitation), 0, 100);
        GuiSliderBar((Rectangle) { editorData.anchor.x + 96, editorData.anchor.y + 344, 384, 32 }, "Physics FPS", EDITOR_DATA(editorData.physicsFrames), 0, 100);
        GuiGroupBox((Rectangle) { editorData.anchor.x + 24, editorData.anchor.y + 400, 472, 52 }, "Springs");
        GuiSliderBar((Rectangle) { editorData.anchor.x + 104, editorData.anchor.y + 420, 102, 16 }, "Stiffness", EDITOR_DATA(editorData.stiffness), 0, 100);
        GuiSliderBar((Rectangle) { editorData.anchor.x + 352, editorData.anchor.y + 420, 102, 16 }, "Desired Length", EDITOR_DATA(editorData.desiredLengthModifier), 0, 5);
        if (GuiButton((Rectangle) { editorData.anchor.x + 24, editorData.anchor.y + 460, 232, 40 }, "Reset Simulation"))
        {
            editorData.deleteThings = !editorData.deleteThings;
        }
        if (GuiButton((Rectangle) { editorData.anchor.x + 264, editorData.anchor.y + 460, 232, 40 }, "Toggle Simulation"))
        {
            editorData.playing = !editorData.playing;
        }
        if (GuiDropdownBox((Rectangle) { editorData.anchor.x + 48, editorData.anchor.y + 80, 184, 32 }, "STATIC;KINEMATIC;DYNAMIC", & editorData.bodyType, editorData.bodyTypeEditMode)) editorData.bodyTypeEditMode = !editorData.bodyTypeEditMode;
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