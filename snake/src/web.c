#include <raylib.h>                                                                                                                                
#include "screen_gameplay.c"
#include <emscripten/emscripten.h>
void drawGameplay();
static const int screenWidth = 1600;
static const int screenHeight = 800;
int main(void){
        InitWindow(screenWidth, screenHeight, "MOFAJIR");
        InitAudioDevice();
        SetTargetFPS(60);
        initGameplayScreen();
	emscripten_set_main_loop(drawGameplay, 0, 1);



        CloseWindow();          // Close window and OpenGL context
        return 0;
}
void drawGameplay(){
                UpdateGameplayScreen();
                DrawGameplayScreen();


}    
