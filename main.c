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

Vector2 GetFarthestVertex(Shape s, Vector2D dir);

int GjkShapesAreColliding(Shape s1, Shape s2);

Vector2 GjkSupport(Shape s1, Shape s2, Vector2 dir);

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

int GjkShapesAreColliding(Shape s1, Shape s2) {
    Vector2 search_dir = Vector2One(); // arbitrary

    Vector2 enclosure[4];
    size_t enclosure_index = 0;

    Vector2 s = GjkSupport(s1, s2, search_dir);
    search_dir = Vector2Negate(s);

    // TODO: FINISH

}

Vector2 GjkSupport(Shape s1, Shape s2, Vector2 dir) {
    Vector2 farthestS1 = Vector2Add(s1.pos, GetFarthestVertex(s1));
    Vector2 farthestS2 = Vector2Add(s2.pos, GetFarthestVertex(s2));
    
    return Vector2Subtract(farthestS1, farthestS2);
}

Vector2 GetFarthestVertex(Shape shape, Vector2D dir) {
    if (shape.verticesCount <= 0) return Vector2Zero();

    Vector2 max = shape.vertices[0];
    int dot_max = Vector2DotProduct(max, dir);

    for (int i = 1; i < shape.verticesCount; ++i) {
        int next_dot = Vector2DotProduct(shape.vertices[i];
        if (next_dot > dot_max)) {
            max_dot = next_dot;
            max = shape.vertices[i];
        }
    }

    return max;
}
