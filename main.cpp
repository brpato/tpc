#include <iostream>
#include <raylib.h>
#include "MacInput.h"

using namespace std;

Color backgroundColor = WHITE;
double lastInputTime = -1.0;
bool backspace = false;
bool click = false;
bool dragging = false;
Vector2 dragOffset;
Texture2D idle;
Texture2D back;



void input(){
    if(globalBackspacePressed()){
        lastInputTime = GetTime();
        backspace = true;
        click = false;
    }
    else if(globalKeyPressed()){
        lastInputTime = GetTime();
        backspace = false;
        click = false;
    }
    else if(globalMousePressed()){
        lastInputTime = GetTime();
        backspace = false;
        click = true;
    }
    if(globalKeyDown()){
        lastInputTime = GetTime();
    }
    if(globalMouseDown()){
        lastInputTime = GetTime();
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

void move(){
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        dragging = true;
        Vector2 window = GetWindowPosition();
        dragOffset.x = globalMouseX() - window.x;
        dragOffset.y = globalMouseY() - window.y;
    }

    if(dragging){
        SetWindowPosition(globalMouseX() - dragOffset.x, globalMouseY() - dragOffset.y);
    }

    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
        dragging = false;
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
    startGlobalInput();
    while(!WindowShouldClose()){
        input();
        move();
        draw();
    }
    CloseWindow();
}

int main(){
    run();
}