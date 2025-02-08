#include <stdio.h>
#include <raylib.h>
#include "screens.h"
#include <stdint.h>
/*
*	Module variables
*/
// Snack structure
#define MAX_SNACK_SIZE 256
#define snake_scale 0.03125f
#define point_scale 0.03125f
#define snake_thickness 64
#define border_thickness 15.0f
#define point_thickness 32
#define revive_cooldown 0.5f
#define nbr_skins 2
#define skin_width 640
// png 1024x1024 // render size 32x32
typedef struct {
	Texture2D blob;
	Texture2D texHead;
	int skinHead;
	Rectangle skinRecHead;
	int32_t head;
	int32_t size;
	int32_t gain;
	float step;
	bool alive;
	int8_t direction;// U:up , D:down , R:right , L:left
	Color color;
	Rectangle headBnds;
	Vector2 position[MAX_SNACK_SIZE];
	Sound dead;
	Sound moveSound;
} Snack;
Snack snake ;
// point structure
typedef struct {
	Vector2 position;
	Color color;
	bool ready;
	Rectangle bounds;
	Texture2D tex;
} SnackPoint;
SnackPoint point;

float dt ;// used for snake movement

int screenW;
int screenH;
Font scoreFont;
int highScore;
/*
	Module functions
*/
void startGame(Snack *snack);
void generatePoint(SnackPoint *point);
bool checkCollision(Snack snack);
void setDirection(Snack *snack );
void moveSnack(Snack *snack);
void updateSnackSize(Snack *snack);


void initGameplayScreen(void){
	snake.blob = LoadTexture("res/blob.png");
	// snake.texHead = LoadTexture("res/head.png");
	snake.texHead = LoadTexture("res/image.png");
	// snake.moveWave = LoadWave("res/sound.wav");
	snake.moveSound = LoadSound("res/music_food.mp3");
	snake.dead = LoadSound("res/dead.mp3");
	point.tex = LoadTexture("res/point.png");
	// load font
	//
	scoreFont = LoadFontEx("res/pixantiqua.ttf", 32, 0, 250);

	snake.skinHead =0;
	snake.skinRecHead = (Rectangle){
		.height = skin_width,
		.width = skin_width,
		.y = 0,
		.x = 0,
	};
	snake.color = RAYWHITE;
	snake.headBnds = (Rectangle){
		.height = snake_thickness,
		.width = snake_thickness,
	};
	screenH = GetScreenHeight();
	screenW = GetScreenWidth();
	startGame(&snake);

	point.bounds = (Rectangle){
		.height = point_thickness,
		.width = point_thickness
	};
	highScore = 0;
}


void UpdateGameplayScreen(void)
{

	if (!snake.alive){
		dt +=GetFrameTime();
		if (dt>= revive_cooldown)startGame(&snake);
	} else {
	// Check for collision
				if (checkCollision(snake)){
					// inGame = false;
					PlaySound(snake.dead);
					if (highScore<snake.size) highScore=snake.size;
					snake.alive = false;					
					dt = 0;
					return;
				}
				// No collisiion
				// set snack direction
				if ((IsKeyDown(KEY_UP)&& snake.direction=='U') || 
					(IsKeyDown(KEY_DOWN) && snake.direction=='D' )|| 
					(IsKeyDown(KEY_RIGHT) && snake.direction=='R' )|| 
					(IsKeyDown(KEY_LEFT)&& snake.direction=='L'))  snake.step = 0.1f;
				else snake.step = 0.4;
				setDirection(&snake);
				// move the snack
				dt += GetFrameTime();
				if (dt>=snake.step){
					moveSnack(&snake);
					// PlaySound(snake.moveSound);
					dt=0;
					// update snack size
					if (snake.head==0){
						updateSnackSize(&snake);

					}
					if (!point.ready) generatePoint(&point);
					else {
						if(CheckCollisionRecs(point.bounds,snake.headBnds)){
							point.ready = false;
							snake.gain++;
							PlaySound(snake.moveSound);

						}				
					}
				
				}
				// Set skin
				if (IsKeyPressed(KEY_S)){
					snake.skinHead= (snake.skinHead+1)%nbr_skins;
					snake.skinRecHead.x=skin_width*snake.skinHead;
				}

	}
}

void DrawGameplayScreen(void)
{
	BeginDrawing();
	//  draw map
	ClearBackground(BLUE);
	DrawLineEx((Vector2){0,0},(Vector2){screenW,0},border_thickness,LIGHTGRAY);
	DrawLineEx((Vector2){0,0},(Vector2){0,screenH},border_thickness,LIGHTGRAY);
	DrawLineEx((Vector2){0,screenH},(Vector2){screenW,screenH},border_thickness,LIGHTGRAY);
	DrawLineEx((Vector2){screenW,0},(Vector2){screenW,screenH},border_thickness,LIGHTGRAY);
	
	//  draw snake
	for(int32_t i=0;i<snake.size;i++){
		/*DrawRectangle(snack.position[k].x,
			snack.position[k].y,
			SNACK_THICKNESS,
			SNACK_THICKNESS,
			snack.color);*/
		if (i==snake.head) DrawTexturePro(snake.texHead, snake.skinRecHead, snake.headBnds, (Vector2){0.0f,0.0f},0, snake.color); 
		else DrawTextureEx(snake.blob,(Vector2){snake.position[i].x,snake.position[i].y},0.0f,snake_scale*4,snake.color);
		
		
	}
	// draw the points
	if (point.ready) DrawTextureEx(point.tex,(Vector2){point.position.x,point.position.y},0.0f,point_scale,RAYWHITE);
	// Draw score:
	DrawTextEx(scoreFont, TextFormat("High Score:%d",highScore), (Vector2){ 30, 30.0f }, (float)scoreFont.baseSize, 2, YELLOW);
	DrawTextEx(scoreFont, TextFormat("Score:%d",snake.size), (Vector2){ 30, 60.0f }, (float)scoreFont.baseSize, 2, YELLOW);

	DrawFPS(15,15);
	EndDrawing();

}











/*	
	* GAME LOGIC FUNCTIONS	
*/
void startGame(Snack *snack){
	dt = 0.0f;
	// Initialise the snack
    snack->head = 0;
    snack->size = 2;
    snack->gain=2;
    snack->alive = true;
	snack->step=0.4;
    snack->position[0]=(Vector2) {320,550};
	snack->position[1]=(Vector2) {320,582};
    snack->direction = 'U';
    // snack->color = ColorFromHSV(GetRandomValue(0,300),0.5f ,0.7f);

	point.ready = false;
	point.color = RED;
}

void generatePoint(SnackPoint *point){
	do {
		point->position.x=GetRandomValue(border_thickness,screenW-border_thickness-point_thickness);
		point->position.y= GetRandomValue(border_thickness,screenH-border_thickness-point_thickness);
	} while (point->position.x==snake.position[snake.head].x && point->position.y==snake.position[snake.head].y);
	point->ready = true;
	point->bounds.x = point->position.x;
	point->bounds.y = point->position.y;

}
bool checkCollision(Snack snack){
	for (int32_t i=1;i<snack.size;i++){
		if (snack.position[snack.head].x==snack.position[i].x && snack.position[snack.head].y==snack.position[i].y && i!=snack.head){
			return true;
		}
	}
	if (snack.position[snack.head].x<border_thickness || snack.position[snack.head].x>screenW-border_thickness || snack.position[snack.head].y<border_thickness || snack.position[snack.head].y>screenH-border_thickness){
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

void moveSnack(Snack *snack){
	int32_t tmp=snack->head;
	snack->head = (snack->head - 1);
	if (snake.head<0)snake.head = snake.size-1;
	switch (snack->direction){
		case 'U':snack->position[snack->head] = (Vector2){snack->position[tmp].x,snack->position[tmp].y-snake_thickness};break;
		case 'D':snack->position[snack->head] = (Vector2){snack->position[tmp].x,snack->position[tmp].y+snake_thickness};break;
		case 'L':snack->position[snack->head] = (Vector2){snack->position[tmp].x-snake_thickness,snack->position[tmp].y};break;
		case 'R':snack->position[snack->head] = (Vector2){snack->position[tmp].x+snake_thickness,snack->position[tmp].y};break;
	}	
	snake.headBnds.x = snack->position[snack->head].x;
	snake.headBnds.y = snack->position[snack->head].y;

}
void updateSnackSize(Snack *snack){
	for (int32_t i =0 ; i<snack->gain;i++){
		snack->position[snack->size] = snack->position[snack->size-1];
		snack->size++;
	}
	snack->gain=0;
}
