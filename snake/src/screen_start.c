#include <raylib.h>
#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

static int loadingPositionX = 0;
static int loadingPositionY = 0;

static int loadingCount = 0;

static int topSideRecWidth = 0;
static int leftSideRecHeight = 0;

static int bottomSideRecWidth = 0;
static int rightSideRecHeight = 0;

Vector2 mousePoint;

#define buttonNumberFrames 3
#define buttonWidth 224
#define buttonHeight 318

// MENU BUTTONS
struct {
	Texture2D tex;
	int state;
	bool action;
	Rectangle source  ;
	Rectangle bounds;
	Vector2 position;
} 

buttonStart ={
	.source = { 0, 0, buttonWidth,(float) (buttonHeight /buttonNumberFrames)},
	.action =false,
	.state=0,
	.bounds = {0,0,buttonWidth,buttonHeight/buttonNumberFrames}
	}, 
buttonExit={
	.source = { 0, 0, buttonWidth, (float)(buttonHeight/buttonNumberFrames )},
	.action =false,
	.bounds = {0,0,buttonWidth,buttonHeight/buttonNumberFrames},
	.state=0,
	};


Texture2D loadingFrame;

enum  { LOADING = 0, MENU } state;
static float alpha = 1.0f;         // Useful for fading


// Start Screen Initialization logic
void InitStartScreen(void)
{
    // TODO: Initialize Start screen variables here!
    framesCounter = 0;
    finishScreen = 0;
	loadingCount = 0;
	state = LOADING;
	finishScreen = false;

	loadingFrame = LoadTexture("res/loading_frame.png");
	buttonStart.tex = LoadTexture("res/button_start.png") ;
	buttonExit.tex = LoadTexture("res/button_exit.png");

	buttonExit.bounds.x = (GetScreenWidth()-buttonWidth)/2.0f;
	buttonExit.bounds.y = GetScreenHeight()/2.0f;
	buttonStart.bounds.x = (GetScreenWidth()-buttonWidth)/2.0f;
	buttonStart.bounds.y = GetScreenHeight()/2.0f-buttonHeight/buttonNumberFrames;
	buttonExit.position = (Vector2){(GetScreenWidth()-buttonWidth)/2.0f,GetScreenHeight()/2.0f};	
	buttonStart.position = (Vector2){(GetScreenWidth()-buttonWidth)/2.0f,GetScreenHeight()/2.0f-(buttonHeight/buttonNumberFrames)};
	


	loadingPositionX = GetScreenWidth()/2 - 128;
    loadingPositionY = GetScreenHeight()/2 - 128;

    topSideRecWidth = 16;
    leftSideRecHeight = 16;
    bottomSideRecWidth = 16;
    rightSideRecHeight = 16;
}

// Start Screen Update logic
void UpdateStartScreen(void)
{
    // TODO: Update Start screen variables here!
	if (state == LOADING){
		framesCounter+=5;//++
		if (framesCounter > 180)
		{
			framesCounter = 0;
			state = MENU;
		}
		else if (framesCounter%12==0) loadingCount++;

	}
	else if (state == MENU){

		mousePoint = GetMousePosition();
        buttonExit.action = false;
		buttonStart.action = false;
        if (CheckCollisionPointRec(mousePoint, buttonExit.bounds))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) buttonExit.state = 2;
            else buttonExit.state = 1;

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) buttonExit.action = true;
        }
        else buttonExit.state = 0;

        if (CheckCollisionPointRec(mousePoint, buttonStart.bounds))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) buttonStart.state = 2;
            else buttonStart.state = 1;

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) buttonStart.action = true;
        }
        else buttonStart.state = 0;

		buttonExit.source.y=(float) buttonExit.state *(float)(buttonHeight/buttonNumberFrames);
		buttonStart.source.y=(float) buttonStart.state * (float)(buttonHeight/buttonNumberFrames);

		if (buttonExit.action || buttonStart.action) finishScreen = true;
		
	}
}

// Start Screen Draw logic
void DrawStartScreen(void)
{
	BeginDrawing();
    // TODO: Draw Start screen here!
	if (state == LOADING){
		ClearBackground(LIGHTGRAY);
		DrawTexture(loadingFrame, loadingPositionX, loadingPositionY, RAYWHITE);
		DrawRectangle(loadingPositionX, loadingPositionY,loadingCount*256/15, 50, Fade(SKYBLUE, 0.3f));
	} else {
		ClearBackground(LIGHTGRAY);
		DrawTextureRec(buttonStart.tex, buttonStart.source, buttonStart.position, WHITE); // Draw button frame
		DrawTextureRec(buttonExit.tex, buttonExit.source, buttonExit.position, WHITE); // Draw button frame
	}
	EndDrawing();
}

// Start Screen Unload logic
void UnloadStartScreen(void)
{
    // TODO: Unload Start screen variables here!
	
	UnloadTexture(loadingFrame);
	UnloadTexture(buttonStart.tex) ;
	UnloadTexture(buttonExit.tex);
	
}

// Start Screen should finish?
int FinishStartScreen(void)
{

    return finishScreen;
}