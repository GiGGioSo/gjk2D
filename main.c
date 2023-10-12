#include <stdio.h>
#include <stdbool.h>
#include "raylib.h"
#include "raymath.h"

#define ARR_LEN(x) (sizeof((x)) / sizeof((x)[0]))

#define SAME_DIR(x, y) (Vector2DotProduct((x), (y)) > 0)

typedef struct Shape {
    Vector2 pos;
    Vector2 *vertices;
    int verticesCount;
} Shape;
void DrawShape(Shape shape, Color color);
Vector2 GetFarthestVertex(Shape s, Vector2 dir);

Vector2 Vector2PerpendicularAwayFrom(Vector2 p, Vector2 away);

bool Gjk2DShapesAreColliding(Shape s1, Shape s2);

Vector2 Gjk2DSupport(Shape s1, Shape s2, Vector2 dir);

bool Gjk2DDoSimplex(Vector2 *simplex, int *simplex_index, Vector2 *dir);

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

    Vector2 vertices1[] = {
        {400.f, 200.f},
        {500.f, -300.f},
        {-100.f, -400.f},
        {-400.f, 100.f},
        {-200.f, 500.f},
    };
    Shape s1 = {{0.f, 0.f}, vertices1, ARR_LEN(vertices1)};

    Vector2 vertices2[] = {
        {200.f, 150.f},
        {250.f, -100.f},
        {-50.f, -200.f},
        {-200.f, 50.f},
        {-150.f, 250.f},
    };
    Shape s2 = {{0.f, 0.f}, vertices2, ARR_LEN(vertices2)};

    Vector2 mouse_in_screen = Vector2Zero();
    Vector2 mouse_in_camera = Vector2Zero();

    while (!WindowShouldClose()) {

        float dt = GetFrameTime();
        float time = GetTime();

        float mouseWheel = GetMouseWheelMove();
        if (mouseWheel > 0) {
            camera.zoom *= 1.1f;
        } else if (mouseWheel < 0) {
            camera.zoom *= 0.9f;
        }

        if (IsCursorOnScreen) {
            mouse_in_screen = GetMousePosition();
            mouse_in_camera = GetScreenToWorld2D(mouse_in_screen, camera);
            s2.pos = mouse_in_camera;
        }

        BeginDrawing();
        BeginMode2D(camera);

        ClearBackground(RAYWHITE);
        DrawShape(s1, BLACK);

        if (Gjk2DShapesAreColliding(s1, s2)) {
            DrawShape(s2, GREEN);
        } else {
            DrawShape(s2, RED);
        }

        EndMode2D();
        DrawFPS(2, 2);
        EndDrawing();
    }
}

void DrawShape(Shape shape, Color color) {
    for (int i = 0; i < shape.verticesCount; ++i) {
        shape.vertices[i] = Vector2Add(shape.vertices[i], shape.pos);
    }
    DrawTriangleFan(shape.vertices, shape.verticesCount, color);
    for (int i = 0; i < shape.verticesCount; ++i) {
        shape.vertices[i] = Vector2Subtract(shape.vertices[i], shape.pos);
    }
}

bool Gjk2DShapesAreColliding(Shape s1, Shape s2) {
    Vector2 search_dir = Vector2One(); // arbitrary

    Vector2 simplex[3];
    int simplex_index = 0;

    Vector2 initial_point = Gjk2DSupport(s1, s2, search_dir);
    simplex[simplex_index++] = initial_point;
    search_dir = Vector2Negate(initial_point);

    while(true) {
        Vector2 next_point = Gjk2DSupport(s1, s2, search_dir);
        if (!SAME_DIR(next_point, search_dir)) return false;

        simplex[simplex_index++] = next_point;

        if (Gjk2DDoSimplex(simplex, &simplex_index, &search_dir)) return true;
    }
}

bool Gjk2DDoSimplex(Vector2 *simplex, int *simplex_index, Vector2 *dir) {
    if (*simplex_index == 2) {
        Vector2 B = simplex[0];
        Vector2 A = simplex[1];
        Vector2 AB = Vector2Subtract(B, A);
        Vector2 AO = Vector2Negate(A);

        if (SAME_DIR(AB, AO)) {
            // Origin is near the shaft
            Vector2 ABperp = Vector2PerpendicularAwayFrom(AB, A);
            *dir = ABperp;
        } else {
            // Origin is near A
            simplex[0] = A;
            *simplex_index = 1;
            *dir = AO;
        } // Origin cannot be near B

        // If the simplex has 2 vertices, it cannot CONTAIN the origin, so false
        return false;

    } else if (*simplex_index == 3) {
        Vector2 C = simplex[0];
        Vector2 B = simplex[1];
        Vector2 A = simplex[2];
        Vector2 AB = Vector2Subtract(B, A);
        Vector2 AC = Vector2Subtract(C, A);
        Vector2 AO = Vector2Negate(A);
        Vector2 ACperp = Vector2PerpendicularAwayFrom(AC, AB);
        Vector2 ABperp = Vector2PerpendicularAwayFrom(AB, AC);

        if (SAME_DIR(ACperp, AO)) {
            if (SAME_DIR(AC, AO)) {
                simplex[1] = A;
                *simplex_index = 2;
                // In the direction of the origin, perpendicular to AC
                *dir = Vector2PerpendicularAwayFrom(AC, A);
                return false;
            } else if (SAME_DIR(AB, AO)) {
                simplex[0] = B;
                simplex[1] = A;
                *simplex_index = 2;
                // In the direction of the origin, perpendicular to AB
                *dir = Vector2PerpendicularAwayFrom(AB, A);
            } else {
                simplex[0] = A;
                *simplex_index = 1;
                *dir = AO;
            }
        } else if (SAME_DIR(ABperp, AO)) {
            if (SAME_DIR(AB, AO)) {
                simplex[0] = B;
                simplex[1] = A;
                *simplex_index = 2;
                // In the direction of the origin, perpendicular to AB
                *dir = Vector2PerpendicularAwayFrom(AB, A);
            } else {
                simplex[0] = A;
                *simplex_index = 1;
                *dir = AO;
            }
        } else {
            // INSIDE THE TRIANGLE, THE ORIGIN IS CONTAINED IN THE SIMPLEX
            return true;
        }
        return false;
    }

    return false;
}

Vector2 Gjk2DSupport(Shape s1, Shape s2, Vector2 dir) {
    Vector2 farthestS1 =
        Vector2Add(s1.pos,
                   GetFarthestVertex(s1, dir));
    Vector2 farthestS2 =
        Vector2Add(s2.pos,
                   GetFarthestVertex(s2, Vector2Negate(dir)));
    
    return Vector2Subtract(farthestS1, farthestS2);
}

Vector2 Vector2PerpendicularAwayFrom(Vector2 p, Vector2 away) {
    Vector2 result = { p.y, -p.x };
    if (SAME_DIR(result, away)) {
        return Vector2Negate(result);
    } else {
        return result;
    }
}

Vector2 GetFarthestVertex(Shape shape, Vector2 dir) {
    if (shape.verticesCount <= 0) return Vector2Zero();

    Vector2 max = shape.vertices[0];
    int max_dot = Vector2DotProduct(max, dir);

    for (int i = 1; i < shape.verticesCount; ++i) {
        int next_dot = Vector2DotProduct(shape.vertices[i], dir);
        if (next_dot > max_dot) {
            max_dot = next_dot;
            max = shape.vertices[i];
        }
    }

    return max;
}
