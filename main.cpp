#include <iostream>
#include <raylib.h>
#include "GlobalInput.h"
#include <string>
#include <vector>

using namespace std;

static double lastInputTime = -1.0;
static Vector2 dragOffset;
static vector<Texture2D> imgs;
static Texture2D currentFrame;
static bool dragging = false;
static size_t frameIndex = 0;

#ifdef _WIN32
void keepWindowTopmost(void* windowHandle);
#endif

static void input(){
    if(globalKeyPressed(Key::ANY) || globalMousePressed()) frameIndex = (frameIndex + 1) % imgs.size();
    currentFrame = imgs[frameIndex];
}
static void move(){
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        dragging = true;
        Vector2 window = GetWindowPosition();
        dragOffset.x = globalMouseX() - window.x;
        dragOffset.y = globalMouseY() - window.y;
    }

    if(dragging) SetWindowPosition(globalMouseX() - dragOffset.x, globalMouseY() - dragOffset.y);

    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) dragging = false;
}

//732x700
static void draw(){
    float x = (GetScreenWidth() - 157) / 2.0f;
    float y = (GetScreenHeight() - 150) / 2.0f;
    BeginDrawing();
    ClearBackground(BLANK);
    DrawTexturePro(currentFrame, {0, 0, (float)currentFrame.width, (float)currentFrame.height},{x, y, 157, 150}, {0, 0}, 0, WHITE);
    EndDrawing();
}

static void run(){
    requestInputPermissions();
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST);
    InitWindow(157, 150, "hi");
    SetTargetFPS(60);
    const std::string assets = std::string(resourcePath()) + "/assets/";

    //needs to be in order of Frame enum
    vector<string> filenames = {
        "cat-1.png", "cat-2.png", "cat-3.png", "cat-4.png", "cat-5.png",
        "cat-6.png", "cat-7.png", "cat-8.png"
    };
    for(const string& s : filenames) imgs.push_back(LoadTexture((assets + s).c_str()));

    startGlobalInput();
    while(!WindowShouldClose()){
        input();
        move();
#ifdef _WIN32
        keepWindowTopmost(GetWindowHandle());
#endif
        draw();
    }
    for(const Texture2D& t : imgs) UnloadTexture(t);
    CloseWindow();
}

int main(){
    run();
}