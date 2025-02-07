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
	Texture2D texture;
	uint32_t head;
	uint32_t size;
	uint32_t gain;
	float step;
	uint8_t direction;// U:up , D:down , R:right , L:left
	Color color;
	Vector2 position[MAX_SNACK_SIZE];
} Snack;
typedef struct {
	Vector2 position;
	Color color;
	bool ready;
} SnackPoint;

/*	General functions	*/
void startGame(Snack *snack);
void generatePoint(SnackPoint *point);
bool checkCollision(Snack snack);
void moveSnack(Snack *snack);
void updateSnackSize(Snack *snack);
void setDirection(Snack *snack );

int main(){
    SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(SCREEN_WIDTH , SCREEN_HEIGHT , "OSIM");
	SetTargetFPS(60);

    // Initialise the snack
    SnackPoint point;
    point.color = RED;
    Snack snack;
	snack.texture = LoadTexture("snake.png");
    // startGame(&snack);
    bool inGame = false;
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
			if (inGame){
				// Check for collision
				if (checkCollision(snack)){
					// inGame = false;
					startGame(&snack);
				} else{
					// set snack direction
					if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT)) snack.step = 0;
					else snack.step = 0.4;
					setDirection(&snack);
					// move the snack
					dt += GetFrameTime();
					if (dt>=snack.step){
						moveSnack(&snack);
						dt=0;
					}
					// update snack size
					if (snack.head==0){
						updateSnackSize(&snack);

					}

				}
			}	else {
				startGame(&snack);
				inGame = true;
			}

			// Draw the snack
			for(uint32_t i=0;i<snack.size;i++){
				int k = (snack.head+i) % snack.size;
				/*DrawRectangle(snack.position[k].x,
					snack.position[k].y,
					SNACK_THICKNESS,
					SNACK_THICKNESS,
					snack.color);*/
				DrawTexture(snack.texture,snack.position[k].x,snack.position[k].y,snack.color);
			}
			if (!point.ready) generatePoint(&point);
			DrawRectangle(point.position.x,point.position.y,20,20,point.color);
			if(snack.position[snack.head].x==point.position.x && snack.position[snack.head].y==point.position.y){
				point.ready = false;
				snack.gain++;
			}
            DrawText("Use arrow keys to move the ball", 500, 10, 20, DARKGRAY);
			DrawFPS(10,10);
		EndDrawing();
	    }
	CloseWindow();
	return EXIT_SUCCESS;
}

void generatePoint(SnackPoint *point){
	point->position.x=20* GetRandomValue(1,25);
	point->position.y= 50 + 20*GetRandomValue(0,43);
	point->ready = true;

}
bool checkCollision(Snack snack){
	for (uint32_t i=1;i<snack.size;i++){
		if (snack.position[snack.head].x==snack.position[i].x && snack.position[snack.head].y==snack.position[i].y && i!=snack.head){
			return true;
		}
	}
	if (snack.position[snack.head].x<20 || snack.position[snack.head].x>SCREEN_WIDTH-380 || snack.position[snack.head].y<50 || snack.position[snack.head].y>SCREEN_HEIGHT-50){
		return true;
	}
	return false;
}
void setDirection(Snack *snack ){
	//Update snack direction
	if(IsKeyPressed(KEY_UP)) snack->direction='U';
	if(IsKeyPressed(KEY_DOWN)) snack->direction='D';
	if(IsKeyPressed(KEY_RIGHT)) snack->direction='R';
	if(IsKeyPressed(KEY_LEFT)) snack->direction='L';	
}
void startGame(Snack *snack){
	// Initialise the snack
    snack->head = 0;
    snack->size = 2;
    snack->gain=2;
	snack->step=0.4;
    snack->position[0]=(Vector2) {320,550};
	snack->position[1]=(Vector2) {320,551};
    snack->direction = 'U';
    snack->color = ColorFromHSV(GetRandomValue(0,300),0.5f ,0.7f);
}
void moveSnack(Snack *snack){
	uint32_t tmp=snack->head;
	snack->head = (snack->head+1) % snack->size;
	switch (snack->direction){
		case 'U':snack->position[snack->head] = (Vector2){snack->position[tmp].x,snack->position[tmp].y-SNACK_THICKNESS};break;
		case 'D':snack->position[snack->head] = (Vector2){snack->position[tmp].x,snack->position[tmp].y+SNACK_THICKNESS};break;
		case 'L':snack->position[snack->head] = (Vector2){snack->position[tmp].x-SNACK_THICKNESS,snack->position[tmp].y};break;
		case 'R':snack->position[snack->head] = (Vector2){snack->position[tmp].x+SNACK_THICKNESS,snack->position[tmp].y};break;
	}	
}
void updateSnackSize(Snack *snack){
	for (uint32_t i =0 ; i<snack->gain;i++){
		snack->position[snack->size] = snack->position[snack->size-1];
		snack->size++;
	}
	snack->gain=0;
}