#include <iostream>
#include <raylib.h>
#include "GlobalInput.h"
#include <string>

using namespace std;

Color backgroundColor = WHITE;
double lastInputTime = -1.0;
bool backspace = false;
bool click = false;
bool dragging = false;
Vector2 dragOffset;
Texture2D idle;
Texture2D back;
Texture2D mouse;
Texture2D key1;
Texture2D key2;
Texture2D currentFrame;
bool typingFrame = false;

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
        typingFrame = !typingFrame;
    }
    else if(globalMousePressed()){
        lastInputTime = GetTime();
        backspace = false;
        click = true;
    }
    if(globalKeyDown()){
        lastInputTime = GetTime();
        click = false;
    }
    if(globalMouseDown()){
        lastInputTime = GetTime();
        click = true;
        backspace = false;
    }

    if(GetTime() - lastInputTime < 0.1){
        if(backspace){
            currentFrame = back;
        }
        else if(click){
            currentFrame = mouse;
        }
        else{
            if(typingFrame){
                currentFrame = key1;
            }
            else{
                currentFrame = key2;
            }
        }
    }
    else{
        currentFrame = idle;
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
    float x = (GetScreenWidth() - 165) / 2.0f;
    float y = (GetScreenHeight() - 150) / 2.0f;
    BeginDrawing();
    ClearBackground(BLANK);
    DrawTexturePro(currentFrame, {0, 0, (float)currentFrame.width, (float)currentFrame.height},{x, y, 165, 150}, {0, 0}, 0, WHITE);
    EndDrawing();
}

void run(){
    requestInputPermissions();
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST);
    InitWindow(165, 165, "hi");
    SetTargetFPS(60);
    std::string assets = std::string(resourcePath()) + "/assets/";
    mouse = LoadTexture((assets + "1.png").c_str());
    back = LoadTexture((assets + "4.png").c_str());
    idle = LoadTexture((assets + "basic.png").c_str());
    key1 = LoadTexture((assets + "left.png").c_str());
    key2 = LoadTexture((assets + "right.png").c_str());

    startGlobalInput();
    while(!WindowShouldClose()){
        input();
        move();
        draw();
    }
    UnloadTexture(mouse);
    UnloadTexture(back);
    UnloadTexture(idle);
    UnloadTexture(key1);
    UnloadTexture(key2);
    CloseWindow();
}

int main(){
    run();
}