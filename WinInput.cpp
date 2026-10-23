#include "GlobalInput.h"
#include <windows.h>
#include <unordered_set>
#include <string>

static std::unordered_set<unsigned short> heldKeys;
static const unordered_set<int> leftKeys = {
    'Q', 'W', 'E', 'R', 'T',
    'A', 'S', 'D', 'F', 'G',
    'Z', 'X', 'C', 'V', 'B'
};
static const unordered_set<int> rightKeys = {
    'Y', 'U', 'I', 'O', 'P',
    'H', 'J', 'K', 'L',
    'N', 'M'
};


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

static int getKeyCode(const Key key){
    switch(key){
        case Key::ONE: return '1';
        case Key::HYPHEN: return VK_OEM_MINUS;
        case Key::ENTER: return VK_RETURN;
        case Key::BACKSPACE: return VK_BACK;
        case Key::SLASH: return VK_OEM_2;
        default: return -1;
    }
}

bool globalKeyPressed(const Key key){
    if(key == Key::ANY){
        bool newPress = false;
        for(int vk = 1; vk <= 255; vk++){
            if(vk == VK_BACK || vk == VK_LBUTTON || vk == VK_RBUTTON || vk == VK_MBUTTON || vk == VK_RETURN || vk == VK_OEM_MIUNS){
                continue;
            }
            bool down = GetAsyncKeyState(vk) & 0x8000;
            if(down){
                auto result = heldKeys.insert(vk);
                if(result.second){
                    newPress = true;
                }
            }
            else{
                heldKeys.erase(vk);
            }
        }
        return newPress;
    }
    const int code = getKeyCode(key);
    if(code == -1) return false;
    bool down = GetAsyncKeyState(code) & 0x8000;
    if(down) return heldKeys.insert(code).second;
    heldKeys.erase(code);
    return false;
}

bool globalKeyDown(const Key key){
    if(key == Key::ANY){
        for(int code = 1; code <= 255; code++){
            if(code != VK_LBUTTON && code != VK_RBUTTON && code != VK_MBUTTON && (GetAsyncKeyState(code) & 0x8000)){
                return true;
            }
        }
        return false;
    }
    if(key == Key::SHIFT) return (GetAsyncKeyState(VK_LSHIFT) & 0x8000) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000);

    const int code = getKeyCode(key);
    if(code == -1) return false;
    return (GetAsyncKeyState(code) & 0x8000) != 0;
}

static KeySide getKeySide(int KeyCode){
    if(leftKeys.contains(keyCode)) return KeySide::LEFT;
    if(rightKeys.contains(keyCode)) return KeySide::RIGHT;
    return KeySide::NONE;
}