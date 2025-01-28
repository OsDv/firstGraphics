#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 1080 
#define SCREEN_HEIGHT 1920/2 
#define CIRC_RADIUS 8
#define CIRC_NUM 10
#define SUB_STEPS 30

int draw_limit ;
int frames_counter;
int main(){
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(SCREEN_WIDTH , SCREEN_HEIGHT , "OSIM");
	SetTargetFPS(60);

	Vector2 gravity = {0.f , 10};
	Vector2 circPos[CIRC_NUM];
	Vector2 circVel[CIRC_NUM];	
	Vector2 circAcc[CIRC_NUM]={gravity.x , gravity.y};
		Color circColor[CIRC_NUM];
		// Initialisations
		for (int i = 0; i<CIRC_NUM ;i++){
			circPos[i] = (Vector2) {GetRandomValue(4*CIRC_RADIUS , 10*CIRC_RADIUS),10*CIRC_RADIUS};
			circVel[i] = (Vector2) {GetRandomValue(100,300), GetRandomValue(1,100)};
			circColor[i] = ColorFromHSV(GetRandomValue(0,300),0.5f ,0.7f);
			
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
				circPos[i].x = circPos[i].x + circVel[i].x*dt + 0.5f * circAcc[i].x *sqrt(dt) ;
				circPos[i].y = circPos[i].y + circVel[i].y*dt + 0.5f * circAcc[i].y *sqrt(dt) ;
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
					circVel[i].y = - circVel[i].y * 0.8f;
				}
				if (circPos[i].y <= CIRC_RADIUS){
					circPos[i].y = CIRC_RADIUS;
					circVel[i].y = - circVel[i].y * 0.8f;				
				}
				
			}
			}
			for (int i=0;i<CIRC_NUM;i++){

				DrawCircle(circPos[i].x , circPos[i].y , CIRC_RADIUS , circColor[i]);				
			}
					
			DrawFPS(10,10);
		EndDrawing();
	}
	CloseWindow();
	return EXIT_SUCCESS;
}
