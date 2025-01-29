#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 1080 
#define SCREEN_HEIGHT 1920/2 
#define CIRC_RADIUS 15
#define SUB_STEPS 15
#define ACC 10000

void updateAcceleration(Vector2 *acc);

int main(){
    SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(SCREEN_WIDTH , SCREEN_HEIGHT , "OSIM");
	SetTargetFPS(60);

    // Initialise the circle
    Vector2 circPos = (Vector2) {SCREEN_WIDTH/2,SCREEN_HEIGHT/2};
    Vector2 circVel = (Vector2) {0,0};
    Vector2 circAcc = (Vector2) {0,0};
    Color circColor = ColorFromHSV(GetRandomValue(0,300),0.5f ,0.7f);
    

    	while (!WindowShouldClose()){
		BeginDrawing();
			ClearBackground(BLACK);

            // update cicle properties
            float dt = GetFrameTime()/SUB_STEPS;
            circVel =(Vector2) {0,0};
            for (int i = 0; i<SUB_STEPS; i++){
                					// check colosion with walls
					if (circPos.x >= GetScreenWidth() - CIRC_RADIUS){
						circPos.x = GetScreenWidth() - CIRC_RADIUS;
						circVel.x = - circVel.x * 0.8f;
					}
					if (circPos.x <= CIRC_RADIUS){
						circPos.x = CIRC_RADIUS;
					}
					if (circPos.y >= GetScreenHeight() - CIRC_RADIUS){
						circPos.y = GetScreenHeight() - CIRC_RADIUS;
					}
					if (circPos.y <= CIRC_RADIUS){
						circPos.y = CIRC_RADIUS;
					}

                updateAcceleration(&circAcc);
                // Update velocity and position
                circVel.x =circVel.x + circAcc.x*dt;
                circVel.y =circVel.y + circAcc.y*dt;
                circPos.x = circPos.x + circVel.x*dt + 0.5f * circAcc.x *dt*dt ;
                circPos.y = circPos.y + circVel.y*dt + 0.5f * circAcc.y *dt*dt ;
            }




            DrawCircleV(circPos, CIRC_RADIUS, circColor);
            DrawText("Use arrow keys to move the ball", 500, 10, 20, DARKGRAY);
            DrawText(TextFormat("Ball position: (%.2f, %.2f)", circPos.x, circPos.y), 10, 40, 20, DARKGRAY);
			DrawFPS(10,10);
		EndDrawing();
	    }
	CloseWindow();
	return EXIT_SUCCESS;
}

void updateAcceleration(Vector2 *acc){
    acc->x = 0;
    acc->y = 0;
    if (IsKeyDown(KEY_UP)){
        acc->y = -ACC;
    }
    if (IsKeyDown(KEY_DOWN)){
        acc->y = ACC;
    }
    if (IsKeyDown(KEY_LEFT)){
        acc->x = -ACC;
    }
    if (IsKeyDown(KEY_RIGHT)){
        acc->x = ACC;
    }
}