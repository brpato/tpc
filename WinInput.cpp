#include "GlobalInput.h"
#include <windows.h>
#include <unordered_set>
#include <string>

static std::unordered_set<unsigned short> heldKeys;

void startGlobalInput(){
}

bool globalKeyPressed(){
    bool newPress = false;
    for(int key = 1; key <= 255; key++) {
        if(key == VK_BACK || key == VK_LBUTTON || key == VK_RBUTTON || key == VK_MBUTTON){
            continue;
        }
        bool down = GetAsyncKeyState(key) & 0x8000;

        if(down){
            auto result = heldKeys.insert(key);

            if(result.second){
                newPress = true;
            }
        }
        else{
            heldKeys.erase(key);
        }
    }
    return newPress;
}

bool globalBackspacePressed(){
    return GetAsyncKeyState(VK_BACK) & 0x0001;
}

bool globalMousePressed(){
    return (GetAsyncKeyState(VK_LBUTTON) & 0x0001) || (GetAsyncKeyState(VK_RBUTTON) & 0x0001) || (GetAsyncKeyState(VK_MBUTTON) & 0x0001);
}

double globalMouseX(){
    POINT mouse;
    GetCursorPos(&mouse);
    return mouse.x;
}

double globalMouseY(){
    POINT mouse;
    GetCursorPos(&mouse);
    return mouse.y;
}

bool globalKeyDown(){
    for(int key = 1; key <= 255; key++) {
        if(key != VK_LBUTTON && key != VK_RBUTTON && key != VK_MBUTTON && (GetAsyncKeyState(key) & 0x8000)){
            return true;
        }
    }
    return false;
}

bool globalMouseDown(){
    return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) || (GetAsyncKeyState(VK_RBUTTON) & 0x8000) || (GetAsyncKeyState(VK_MBUTTON) & 0x8000);
}

const char* resourcePath(){
    static std::string path;

    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);

    path = buffer;

    size_t lastSlash = path.find_last_of("\\/");
    path = path.substr(0, lastSlash);

    return path.c_str();
}

void requestInputPermissions(){
}