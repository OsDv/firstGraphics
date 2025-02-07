#include <raylib.h>
// #include "screens.h"
// #include "screen_start.c"
#include "screen_gameplay.c"
static const int screenWidth = 1600;
static const int screenHeight = 800;

int main(void){
	InitWindow(screenWidth, screenHeight, "MOFAJIR");
	InitAudioDevice();
	SetTargetFPS(60);
	// InitStartScreen();
	initGameplayScreen();

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        
		// UpdateDrawFrame();
		// UpdateStartScreen();
		// DrawStartScreen();

		UpdateGameplayScreen();
		DrawGameplayScreen();

		
		
		// end of drawing
		
    }
	// UnloadStartScreen();
	
	
	CloseWindow();          // Close window and OpenGL context
	return 0;
}

