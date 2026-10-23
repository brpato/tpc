#include <iostream>
#include <raylib.h>
#include "GlobalInput.h"
#include <string>
#include <vector>

using namespace std;

static Color backgroundColor = WHITE;
static double lastInputTime = -1.0;
static bool backspace = false;
static bool click = false;
static bool dragging = false;
static bool enter = false;
static bool hyphen = false;
static Vector2 dragOffset;
//static Texture2D idle;
//static Texture2D backk;
//static Texture2D mouseb;
//static Texture2D leftk;
//static Texture2D rightk;
//static Texture2D entk;
//static Texture2D hyphk;
static vector<Texture2D> imgs;
static Texture2D currentFrame;
static bool typingFrame = false;

enum class InputState{
    TYPING, BACKSPACE, MOUSE, ENTER, HYPHEN
};

static InputState state = InputState::TYPING;

enum Frame{
    IDLE, BACKSPACE, MOUSE, LEFT, RIGHT, ENTER, HYPHEN
};

#ifdef _WIN32
void keepWindowTopmost(void* windowHandle);
bool isWindowTopmost(void* windowHandle);
#endif

static void input(){
    bool active = false;
    if(globalKeyPressed(Key::BACKSPACE)){
        backspace = true;
        click = false;
        enter = false;
        active = true;
    }
    else if(globalKeyPressed(Key::ENTER)){
        std::cout << "ENTER\n";
        backspace = false;
        click = false;
        enter = true;
        active = true;
    }
    else if(globalKeyPressed(Key::ANY)){
        backspace = false;
        click = false;
        typingFrame = !typingFrame;
        enter = false;
        active = true;
    }
    else if(globalMousePressed()){
        backspace = false;
        click = true;
        enter = false;
        active = true;
    }
    if(globalKeyDown(Key::ANY)){
        click = false;
        active = true;
        enter = globalKeyDown(Key::ENTER);
        backspace = globalKeyDown(Key::BACKSPACE);
    }
    if(globalMouseDown()){
        click = true;
        backspace = false;
        enter = false;
        active = true;
    }

    if(active){
        lastInputTime = GetTime();
    }

    if(GetTime() - lastInputTime < 0.1){
        if(backspace){
            currentFrame = imgs[BACKSPACE];
        }
        else if(click){
            currentFrame = imgs[MOUSE];
        }
        else if(enter){
            currentFrame = imgs[ENTER];
        }
        else{
            if(typingFrame){
                currentFrame = imgs[LEFT];
            }
            else{
                currentFrame = imgs[RIGHT];
            }
        }
    }
    else{
        currentFrame = imgs[IDLE];
    }
}

static void move(){
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

static void draw(){
    float x = (GetScreenWidth() - 165) / 2.0f;
    float y = (GetScreenHeight() - 150) / 2.0f;
    BeginDrawing();
    ClearBackground(BLANK);
    DrawTexturePro(currentFrame, {0, 0, (float)currentFrame.width, (float)currentFrame.height},{x, y, 165, 150}, {0, 0}, 0, WHITE);
    EndDrawing();
}

static void run(){
    requestInputPermissions();
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST);
    InitWindow(165, 165, "hi");
    SetTargetFPS(60);
    const std::string assets = std::string(resourcePath()) + "/assets/";

    //needs to be in order of Frame enum
    vector<string> filenames = {"idle.png", "backk.png", "mouseb.png", "left.png", "right.png", "ent.png"};
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