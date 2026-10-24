#include "GlobalInput.h"
#include <windows.h>
#include <unordered_set>
#include <string>

static std::unordered_set<unsigned short> heldKeys;
static const std::unordered_set<int> leftKeys = {
    'Q','W','E','R','T',
    'A','S','D','F','G',
    'Z','X','C','V','B',
    '1','2','3','4','5',
    // Modifiers and controls
    VK_LSHIFT, VK_LCONTROL, VK_LMENU,
    VK_TAB, VK_CAPITAL, VK_LWIN,
    // Function keys
    VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6,
    // Other
    VK_OEM_3, // ` ~
    VK_ESCAPE
};
static const std::unordered_set<int> rightKeys = {
    'Y','U','I','O','P',
    'H','J','K','L',
    'N','M',
    '6','7','8','9','0',
    // Modifiers and controls
    VK_RSHIFT, VK_RCONTROL, VK_RMENU, VK_RWIN,
    // Function keys
    VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12,
    // Punctuation
    VK_OEM_4, // [ {
    VK_OEM_6, // ] }
    VK_OEM_5, // \ |
    VK_OEM_1, // ; :
    VK_OEM_7, // ' "
    VK_OEM_PLUS, // = +
    VK_OEM_PERIOD, // . >
    VK_OEM_COMMA, // , <
    VK_OEM_2, // /
    // Navigation
    VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
    VK_INSERT, VK_DELETE, VK_HOME, VK_END,
    VK_PRIOR, VK_NEXT, VK_SNAPSHOT,
    // Numpad
    VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2,
    VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5,
    VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8,
    VK_NUMPAD9,
    VK_ADD, // +
    VK_SUBTRACT, // -
    VK_MULTIPLY, // *
    VK_DIVIDE, // /
    VK_DECIMAL, // .
    VK_NUMLOCK // Num Lock
};
static KeySide lastKeySide = KeySide::NONE;

void startGlobalInput(){
}

bool globalMousePressed(){
    return (GetAsyncKeyState(VK_LBUTTON) & 0x0001) || (GetAsyncKeyState(VK_RBUTTON) & 0x0001) || (
               GetAsyncKeyState(VK_MBUTTON) & 0x0001);
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
    return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) || (GetAsyncKeyState(VK_RBUTTON) & 0x8000) || (
               GetAsyncKeyState(VK_MBUTTON) & 0x8000);
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

static KeySide getKeySide(const int KeyCode){
    if(leftKeys.contains(KeyCode)) return KeySide::LEFT;
    if(rightKeys.contains(KeyCode)) return KeySide::RIGHT;
    return KeySide::NONE;
}

KeySide globalKeySidePressed(){
    return lastKeySide;
}

KeySide globalKeySideDown(){
    for(int vk = 1; vk <= 255; vk++){
        if(GetAsyncKeyState(vk) & 0x8000){
            KeySide side = getKeySide(vk);
            if(side != KeySide::NONE) return side;
        }
    }
    return KeySide::NONE;
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
            if(vk == VK_BACK || vk == VK_LBUTTON || vk == VK_RBUTTON || vk == VK_MBUTTON || vk == VK_RETURN || vk ==
               VK_OEM_MINUS){
                continue;
            }
            bool down = GetAsyncKeyState(vk) & 0x8000;
            if(down){
                auto result = heldKeys.insert(vk);
                if(result.second){
                    newPress = true;
                    KeySide side = getKeySide(vk);
                    if(side != KeySide::NONE) lastKeySide = side;
                }
            } else{
                heldKeys.erase(vk);
            }
        }
        return newPress;
    }
    const int code = getKeyCode(key);
    if(code == -1) return false;
    if(GetAsyncKeyState(code) & 0x8000) return heldKeys.insert(code).second;
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
