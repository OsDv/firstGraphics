#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 1080 
#define SCREEN_HEIGHT 1920/2 
#define CIRC_RADIUS 15
#define CIRC_NUM 10
#define SUB_STEPS 30

int draw_limit ;
int frames_counter;
int main(){
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(SCREEN_WIDTH , SCREEN_HEIGHT , "OSIM");
	SetTargetFPS(60);

	Vector2 gravity = {0.f , 500.f};
	Vector2 circPos[CIRC_NUM];
	Vector2 circVel[CIRC_NUM];	
	Vector2 circAcc[CIRC_NUM];
	Color circColor[CIRC_NUM];
		// Initialisations
		for (int i = 0; i<CIRC_NUM ;i++){
			circPos[i] = (Vector2) {GetRandomValue(4*CIRC_RADIUS , 10*CIRC_RADIUS),10*CIRC_RADIUS};
			circVel[i] = (Vector2) {GetRandomValue(100,300), GetRandomValue(1,100)};
			circColor[i] = ColorFromHSV(GetRandomValue(0,300),0.5f ,0.7f);
			circAcc[i] = gravity;
			
		}
		// Open window
		while (!WindowShouldClose()){
		BeginDrawing();
			ClearBackground(BLACK);
			float dt = GetFrameTime()/SUB_STEPS;
			for (int t=0;t<SUB_STEPS;t++){
				// Update circles properties
				for (int i =0;i<CIRC_NUM;i++){
					// Update velocity and position
					circPos[i].x = circPos[i].x + circVel[i].x*dt + 0.5f * circAcc[i].x *dt*dt ;
					circPos[i].y = circPos[i].y + circVel[i].y*dt + 0.5f * circAcc[i].y *dt*dt ;
					circVel[i].x = circVel[i].x + circAcc[i].x*dt;
					circVel[i].y = circVel[i].y + circAcc[i].y*dt;
					// check colosion with walls
					if (circPos[i].x >= GetScreenWidth() - CIRC_RADIUS){
						circPos[i].x = GetScreenWidth() - CIRC_RADIUS;
						circVel[i].x = - circVel[i].x * 0.8f;
					}
					if (circPos[i].x <= CIRC_RADIUS){
						circPos[i].x = CIRC_RADIUS;
						circVel[i].x = - circVel[i].x * 0.8f;				
					}
					if (circPos[i].y >= GetScreenHeight() - CIRC_RADIUS){
						circPos[i].y = GetScreenHeight() - CIRC_RADIUS;
						circVel[i].y = - circVel[i].y * 0.5f;
					}
					if (circPos[i].y <= CIRC_RADIUS){
						circPos[i].y = CIRC_RADIUS;
						circVel[i].y = - circVel[i].y * 0.5f;				
					}
					// Check collision with other circles
					for (int j =i+1 ; j<CIRC_NUM;j++){
						Vector2 dist = Vector2Subtract(circPos[j],circPos[i]);
						float dist_len = Vector2Length(dist);
						if (dist_len <= 2*CIRC_RADIUS){
							Vector2 norm = Vector2Normalize(dist);
							circPos[j] = Vector2Add(circPos[j],Vector2Scale(norm,2*CIRC_RADIUS - dist_len));
							Vector2 temp = circVel[i];
							circVel[i] = circVel[j];
							circVel[j] = temp;
						}
					}

				}

			}
			// Draw circles
			for (int i=0;i<CIRC_NUM;i++){
				
				DrawCircle(circPos[i].x , circPos[i].y , CIRC_RADIUS , circColor[i]);				
			}
					
			DrawFPS(10,10);
		EndDrawing();
	}
	CloseWindow();
	return EXIT_SUCCESS;
}
