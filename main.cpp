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
void launchRightPet();
#endif

static void input(KeySide side){
    bool active = false;
    if(side == KeySide::RIGHT && globalKeyPressed(Key::BACKSPACE)){
        state = InputState::BACKSPACE;
        active = true;
    }
    else if(side == KeySide::RIGHT && globalKeyPressed(Key::ENTER)){
        state = InputState::ENTER;
        active = true;
    }
    else if(side == KeySide::RIGHT && globalKeyPressed(Key::HYPHEN)){
        state = InputState::HYPHEN;
        active = true;
    }
    else if(globalKeyPressed(Key::ANY)){
        KeySide kSide = globalKeySidePressed();
        if(kSide == side){
            typingFrame = (side == KeySide::LEFT);
            state = InputState::TYPING;
            active = true;
        }
    }
    else if(globalMousePressed()){
        state = InputState::MOUSE;
        active = true;
    }

    if(globalKeyDown(Key::ANY)){
        bool matched = true;
        if(globalKeyDown(Key::SHIFT) && globalKeyDown(Key::ONE)) state = InputState::EXCLAMATION;
        else if(globalKeyDown(Key::SHIFT) && globalKeyDown(Key::SLASH)) state = InputState::QUESTION;
        else if(side == KeySide::RIGHT && globalKeyDown(Key::BACKSPACE)) state = InputState::BACKSPACE;
        else if(side == KeySide::RIGHT && globalKeyDown(Key::ENTER)) state = InputState::ENTER;
        else if(side == KeySide::RIGHT && globalKeyDown(Key::HYPHEN)) state = InputState::HYPHEN;
        else if(globalKeySideDown(side)) state = InputState::TYPING;
        else matched = false;
        if(matched) active = true;
        typingFrame = (side == KeySide::LEFT);
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

static void run(KeySide side){
    requestInputPermissions();
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST);
    InitWindow(165, 165, "hi");
    SetTargetFPS(60);
    if(side == KeySide::LEFT) SetWindowPosition(600, 300);
    else SetWindowPosition(300, 300);
    const std::string assets = std::string(resourcePath()) + "/assets/" + (side == KeySide::LEFT ? "left/" : "right/");

    //needs to be in order of Frame enum
    vector<string> filenames = {
        "idle.png", "backk.png", "mouseb.png", "left.png", "right.png",
        "entk.png", "hyphk.png", "excl.png", "ques.png"
    };
    for(const string& s : filenames) imgs.push_back(LoadTexture((assets + s).c_str()));

    startGlobalInput();
    while(!WindowShouldClose()){
        input(side);
        move();
#ifdef _WIN32
        keepWindowTopmost(GetWindowHandle());
#endif
        draw();
    }
    for(const Texture2D& t : imgs) UnloadTexture(t);
    CloseWindow();
}

int main(int argc, char* argv[]){
    if(argc > 1 && string(argv[1]) == "right") run(KeySide::RIGHT);
    else{
#ifdef _WIN32
        if(argc == 1) launchRightPet();
#endif
        run(KeySide::LEFT);
    }
}