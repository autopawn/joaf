#include "raylib.h"

#define GOLDEN_RAD 1.618034

static Color EYE_COLOR = {255, 255, 222, 255};
static const float EYE_SPREAD = 0.55f;

struct facestate
{
    struct eyestate
    {
        // Eye positions from (0.0 to 1.0)
        float x, y;
        // How open are the eyes (0.0 to 1.0)
        float openess;
    } eyes[2];
};

struct facestate INITIAL_FACE_STATE = {
    .eyes = {
        {0.5f - EYE_SPREAD / 2.0f, 0.5f, 1.0f},
        {0.5f + EYE_SPREAD / 2.0f, 0.5f, 1.0f},
    },
};

void draw_face(const struct facestate *face)
{
    // Height (minus limit for mouth)
    float available_h = 0.8f * GetRenderHeight();
    float available_w = GetRenderWidth();

    // Standard eye height
    float eye_h = available_h/GOLDEN_RAD;
    float eye_w = 0.5f * eye_h;

    // Eye center positions
    float pos_x[2] = {0};
    float pos_y[2] = {0};

    for (int i = 0; i < 2; ++i)
    {
        // Translate eye positions to the screen
        pos_x[i] = 0.5f * eye_w + (available_w - eye_w) * face->eyes[i].x;
        pos_y[i] = 0.5f * eye_h + (available_h - eye_h) * face->eyes[i].y;
    }

    for (int i = 0; i < 2; ++i)
    {
        DrawEllipse(pos_x[i], pos_y[i], 0.5f * eye_w, 0.5f * eye_h, EYE_COLOR);
    }

    for (int i = 0; i < 2; ++i)
    {
        const struct eyestate *eye = &face->eyes[i];
        
        if (face->eyes[i].openess > 0.0f)
        {
            float MIN_THICKNESS = 0.25f;
            float thick = 0.5f * eye_w * (MIN_THICKNESS + (1.0f - eye->openess) * (1.0f - MIN_THICKNESS));

            DrawEllipse(pos_x[i], pos_y[i], 0.5f * eye_w - thick, 0.5f * eye_h - thick, BLACK);
        }
    }
}

void interpolate_facestate(struct facestate *face, const struct facestate *target, float remain)
{
    float alpha = 1.0f / (1.0f + remain);

    for (int i = 0; i < 2; ++i)
    {
        face->eyes[i].x = (1.0f - alpha) * face->eyes[i].x + alpha * target->eyes[i].x;
        face->eyes[i].y = (1.0f - alpha) * face->eyes[i].y + alpha * target->eyes[i].y;
        face->eyes[i].openess = (1.0f - alpha) * face->eyes[i].openess + alpha * target->eyes[i].openess;
    }
}

void set_random_facestate(struct facestate *face)
{
    float eye_pos_x = GetRandomValue(0, 1024)/1024.0f * (1.0f - EYE_SPREAD);
    float eye_pos_y = GetRandomValue(0, 1024)/1024.0f;

    float current_openess = face->eyes[0].openess;

    if (current_openess < 0.5)
    {
        if (GetRandomValue(0, 10) == 0)
            current_openess = 1.0f;
    }
    else
    {
        if (GetRandomValue(0, 3) == 0)
            current_openess = 0.0f;
    }

    for (int i = 0; i < 2; ++i)
    {
        face->eyes[i].x = eye_pos_x + i * EYE_SPREAD;
        face->eyes[i].y = eye_pos_y;
        face->eyes[i].openess = current_openess;
    }
}

int main(void)
{
    struct facestate face = INITIAL_FACE_STATE;
    struct facestate target = INITIAL_FACE_STATE;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "JOAF");
    SetTargetFPS(60);

    int frame = 0;
    int interpolation_delay = 0;
    int next_movement = GetRandomValue(10, 30);

    while (!WindowShouldClose())
    {
        if (interpolation_delay > 0)
            --interpolation_delay;
        interpolate_facestate(&face, &target, interpolation_delay);

        if (next_movement > 0)
        {
            --next_movement;
        }
        else
        {
            set_random_facestate(&target);
            interpolation_delay = 3;
            next_movement = GetRandomValue(30, 60);
        }

        BeginDrawing();
            ClearBackground(BLACK);

            draw_face(&face);
        EndDrawing();

        ++frame;
    }

    CloseWindow();

    return 0;
}
