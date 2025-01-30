#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 1000 
#define SUB_STEPS 15
#define ACC 10000
#define MAX_SNACK_SIZE 256
#define SNACK_THICKNESS 20
#define MARGINE_COLOR DARKBLUE
typedef struct {
	uint32_t head;
	uint32_t size;
	uint32_t gain;
	uint8_t direction;// U:up , D:down , R:right , L:left
	Color color;
	Vector2 position[MAX_SNACK_SIZE];
} Snack;

void updateAcceleration(Vector2 *acc);

int main(){
    SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(SCREEN_WIDTH , SCREEN_HEIGHT , "OSIM");
	SetTargetFPS(60);

    // Initialise the snack
    Snack snack;
    snack.head = 0;
    snack.size = 1;
    snack.gain=2;
    snack.position[0]=(Vector2) {320,550};
    snack.direction = 'U';
    snack.color = ColorFromHSV(GetRandomValue(0,300),0.5f ,0.7f);
    
	float dt = 0;
    	while (!WindowShouldClose()){
		BeginDrawing();
			ClearBackground(GRAY);
			DrawRectangle(0,0,SCREEN_WIDTH,50,MARGINE_COLOR);
			DrawRectangle(0,SCREEN_HEIGHT-50,SCREEN_WIDTH,50,MARGINE_COLOR);
			DrawRectangle(0,50,20,SCREEN_HEIGHT-100,MARGINE_COLOR);
			DrawRectangle(SCREEN_WIDTH-380,50,380,SCREEN_HEIGHT-100,MARGINE_COLOR);
			// Make the grid in rectangle (50,50,600,900)
			uint32_t x=20,y=50;
			while(x<=SCREEN_WIDTH-380 ||y <=SCREEN_HEIGHT-50){
				if (x<=SCREEN_WIDTH-380){
					DrawLineV((Vector2){x,50},(Vector2){x,SCREEN_HEIGHT-50},LIGHTGRAY);
					x+=20;
				}
				if (y<=SCREEN_HEIGHT-50){
					DrawLineV((Vector2){20,y},(Vector2){SCREEN_WIDTH-380,y},LIGHTGRAY);
					y+=20;
				}
			}
			// Move The Snack
			//Update snack direction
			if(IsKeyPressed(KEY_UP)) snack.direction='U';
			if(IsKeyPressed(KEY_DOWN)) snack.direction='D';
			if(IsKeyPressed(KEY_RIGHT)) snack.direction='R';
			if(IsKeyPressed(KEY_LEFT)) snack.direction='L';
			if (snack.head==0){
				for (uint32_t i =0 ; i<snack.gain;i++){
					snack.position[snack.size] = snack.position[snack.size-1];
					snack.size++;
				}
				snack.gain=0;

			}
		dt += GetFrameTime();
			if (dt>=0.4){
				uint32_t tmp=snack.head;
				snack.head = (snack.head+1) % snack.size;
				switch (snack.direction){
					case 'U':snack.position[snack.head] = (Vector2){snack.position[tmp].x,snack.position[tmp].y-SNACK_THICKNESS};break;
					case 'D':snack.position[snack.head] = (Vector2){snack.position[tmp].x,snack.position[tmp].y+SNACK_THICKNESS};break;
					case 'L':snack.position[snack.head] = (Vector2){snack.position[tmp].x-SNACK_THICKNESS,snack.position[tmp].y};break;
					case 'R':snack.position[snack.head] = (Vector2){snack.position[tmp].x+SNACK_THICKNESS,snack.position[tmp].y};break;
				}	
				dt=0;
			}
			// Draw the snack
			for(uint32_t i=0;i<snack.size;i++){
				int k = (snack.head+i) % snack.size;
				DrawRectangle(snack.position[k].x,
					snack.position[k].y,
					SNACK_THICKNESS,
					SNACK_THICKNESS,
					snack.color);
			}

            DrawText("Use arrow keys to move the ball", 500, 10, 20, DARKGRAY);
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
