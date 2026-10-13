#include <iostream>
#include <raylib.h>

using namespace std;

Color backgroundColor = WHITE;
double lastInputTime = -1.0;
bool backspace = false;
bool click = false;

void input(){
    if(IsKeyPressed(KEY_BACKSPACE)){
        lastInputTime = GetTime();
        backspace = true;
        click = false;
    }
    else if(GetKeyPressed()){
        lastInputTime = GetTime();
        backspace = false;
        click = false;
    }
    else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
        lastInputTime = GetTime();
        backspace = false;
        click = true;
    }
    if(GetTime() - lastInputTime < 0.1){
        if(backspace){
            backgroundColor = BLUE;
        }
        else if(click){
            backgroundColor = GREEN;
        }
        else{
            backgroundColor = RED;
        }
    }
    else{
        backgroundColor = WHITE;
    }
}

void draw(){
    BeginDrawing();
    ClearBackground(backgroundColor);
    EndDrawing();
}

void run(){
    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST);
    InitWindow(200, 200, "hi");
    while(!WindowShouldClose()){
        input();
        draw();
    }
    CloseWindow();
}

int main(){
    run();
}
