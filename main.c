#include <stdio.h>
#include "raylib.h"
#include "raymath.h"

#define ARR_LEN(x) (sizeof((x)) / sizeof((x)[0]))

typedef struct Shape {
    Vector2 pos;
    Vector2 *vertices;
    int verticesCount;
} Shape;

void DrawShape(Shape shape, float x, float y, Color color);

int ShapesAreColliding(Shape s1, Shape s2);

int main() {

    int width = 900;
    int height = 600;

    InitWindow(width, height, "GJK Algorithm 2D");
    // SetTargetFPS(60);
    SetExitKey(KEY_F4);

    Camera2D camera = {
        .offset = {width/2, height/2},
        .target = {0.f, 0.f},
        .rotation = 0.f,
        .zoom = 1.0f
    };

    Vector2 vertices[] = {
        {300.f, 300.f},
        {300.f, -300.f},
        {-300.f, -300.f},
        {-300.f, 300.f},
    };

    Shape shape = {{0.f, 0.f}, vertices, ARR_LEN(vertices)};

    while (!WindowShouldClose()) {

        float dt = GetFrameTime();
        float time = GetTime();

        float mouseWheel = GetMouseWheelMove();
        if (mouseWheel > 0) {
            camera.zoom *= 1.1f;
        } else if (mouseWheel < 0) {
            camera.zoom *= 0.9f;
        }

        // Vector2 random_pos = {width/2, height/2};
        Vector2 random_pos = Vector2Zero();
        Vector2 origin = GetScreenToWorld2D(random_pos, camera);
        // TraceLog(LOG_INFO, "%f:%f\n", origin.x, origin.y);

        BeginDrawing();
        BeginMode2D(camera);

        ClearBackground(RAYWHITE);
        DrawShape(shape, 0.f, 0.f, RED);

        EndMode2D();
        DrawFPS(2, 2);
        EndDrawing();
    }
}

void DrawShape(Shape shape, float x, float y, Color color) {
    for (int i = 0; i < shape.verticesCount; ++i) {
        shape.vertices[i] = Vector2Add(shape.vertices[i], shape.pos);
    }
    DrawTriangleFan(shape.vertices, shape.verticesCount, color);
    for (int i = 0; i < shape.verticesCount; ++i) {
        shape.vertices[i] = Vector2Subtract(shape.vertices[i], shape.pos);
    }
}

int ShapesAreColliding(Shape s1, Shape s2) {
}
