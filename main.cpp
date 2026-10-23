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
static bool typingFrame = false;
static bool dragging = false;

enum class InputState{
    TYPING, BACKSPACE, MOUSE, ENTER, HYPHEN, EXCLAMATION, QUESTION
};

static InputState state = InputState::TYPING;

enum Frame{
    IDLE, BACKSPACE, MOUSE, LEFT, RIGHT, ENTER, HYPHEN, EXCLAMATION, QUESTION
};

#ifdef _WIN32
void keepWindowTopmost(void* windowHandle);
#endif

static void input(){
    bool active = false;
    if(globalKeyPressed(Key::BACKSPACE)){
        state = InputState::BACKSPACE;
        active = true;
    }
    else if(globalKeyPressed(Key::ENTER)){
        state = InputState::ENTER;
        active = true;
    }
    else if(globalKeyPressed(Key::HYPHEN)){
        state = InputState::HYPHEN;
        active = true;
    }
    else if(globalKeyPressed(Key::ANY)){
        state = InputState::TYPING;
        typingFrame = !typingFrame;
        active = true;
    }
    else if(globalMousePressed()){
        state = InputState::MOUSE;
        active = true;
    }

    if(globalKeyDown(Key::ANY)){
        active = true;
        if(globalKeyDown(Key::SHIFT) && globalKeyDown(Key::ONE)) state = InputState::EXCLAMATION;
        else if(globalKeyDown(Key::SHIFT) && globalKeyDown(Key::SLASH)) state = InputState::QUESTION;
        else if(globalKeyDown(Key::BACKSPACE)) state = InputState::BACKSPACE;
        else if(globalKeyDown(Key::ENTER)) state = InputState::ENTER;
        else if(globalKeyDown(Key::HYPHEN)) state = InputState::HYPHEN;
        else state = InputState::TYPING;
    }

    if(globalMouseDown()){
        state = InputState::MOUSE;
        active = true;
    }

    if(active) lastInputTime = GetTime();

    if(GetTime() - lastInputTime < 0.1){
        if(state == InputState::EXCLAMATION) currentFrame = imgs[EXCLAMATION];
        else if(state == InputState::QUESTION) currentFrame = imgs[QUESTION];
        else if(state == InputState::BACKSPACE) currentFrame = imgs[BACKSPACE];
        else if(state == InputState::ENTER) currentFrame = imgs[ENTER];
        else if(state == InputState::HYPHEN) currentFrame = imgs[HYPHEN];
        else if(state == InputState::MOUSE) currentFrame = imgs[MOUSE];
        else if(state == InputState::TYPING) currentFrame = imgs[typingFrame ? LEFT : RIGHT];
    }
    else currentFrame = imgs[IDLE];
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
    vector<string> filenames = {"idle.png", "backk.png", "mouseb.png", "left.png", "right.png", "ent.png", "hyphk.png", "excl.png", "ques.png"};
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