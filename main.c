#include "lib_phys.h"
#define MAX 4

int main() {
    Shape shapes[MAX];
    shapes[0] = Box(100, 700, 1000, 50);
    shapes[1] = Box(160, 400, 150, 150);
    shapes[2] = Ball(700, 150, 40);
    shapes[3] = Ball(800, 400, 40);
    //e2_applyForce(&shapes[0], (Vector2){0, 0}, 20);

    shapes[0].mass = INFINITY;
    shapes[0].inertia = INFINITY;
    shapes[1].mass = INFINITY;
    shapes[1].inertia = INFINITY;

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 800; 

    InitWindow(screenWidth, screenHeight, "raylib");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKBLUE);
        for (size_t i = 0; i < MAX; i++) {
            for (size_t j = i + 1; j < MAX; j++) {
                checkColl(&shapes[i], &shapes[j]);
            }
            //Vector2 force = e2_checkKick(&shapes[i]);
            Vector2 kick = checkKick(&shapes[i]);
            applyForce(&shapes[i], kick, 0);
            applyFriction(&shapes[i]);
            shapeUpdate(&shapes[i]);
            shapeDraw(&shapes[i], 3, RED);
        }
        //printf("%f\n", shapes[0].velocity.y);
    	EndDrawing();
    }

    CloseWindow();   
	return 0;


    return EXIT_SUCCESS;
}
