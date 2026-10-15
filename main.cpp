#include <iostream>
#include <raylib.h>
#include "GlobalInput.h"
#include <string>

using namespace std;

static Color backgroundColor = WHITE;
static double lastInputTime = -1.0;
static bool backspace = false;
static bool click = false;
static bool dragging = false;
static Vector2 dragOffset;
static Texture2D idle;
static Texture2D backk;
static Texture2D mouseb;
static Texture2D leftk;
static Texture2D rightk;
static Texture2D currentFrame;
static bool typingFrame = false;

#ifdef _WIN32
void keepWindowTopmost(void* windowHandle);
bool isWindowTopmost(void* windowHandle);
#endif

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
            currentFrame = backk;
        }
        else if(click){
            currentFrame = mouseb;
        }
        else{
            if(typingFrame){
                currentFrame = leftk;
            }
            else{
                currentFrame = rightk;
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
    const std::string assets = std::string(resourcePath()) + "/assets/";
    mouseb = LoadTexture((assets + "mouseb.png").c_str());
    backk = LoadTexture((assets + "backk.png").c_str());
    idle = LoadTexture((assets + "idle.png").c_str());
    leftk = LoadTexture((assets + "left.png").c_str());
    rightk = LoadTexture((assets + "right.png").c_str());

    startGlobalInput();
    while(!WindowShouldClose()){
        input();
        move();
#ifdef _WIN32
        keepWindowTopmost(GetWindowHandle());
#endif
        draw();
    }
    UnloadTexture(mouseb);
    UnloadTexture(backk);
    UnloadTexture(idle);
    UnloadTexture(leftk);
    UnloadTexture(rightk);
    CloseWindow();
}

int main(){
    run();
}