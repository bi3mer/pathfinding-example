#include "raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Pathfinding Example");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        /////////////////////// Update ///////////////////////

        /////////////////////// Draw ///////////////////////
        BeginDrawing();

        ClearBackground(BLACK);
        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}