#import <AppKit/AppKit.h>
#import <ApplicationServices/ApplicationServices.h>
#import <IOKit/hidsystem/IOLLEvent.h>
#include "GlobalInput.h"
#include <unordered_set>

static bool keyPressed = false;
static bool backspacePressed = false;
static bool mousePressed = false;
static std::unordered_set<unsigned short> heldKeys;
static std::unordered_set<unsigned short> pressedKeys;
static bool mouseDown = false;

static const std::unordered_set<int> leftKeys = {
    12, 13, 14, 15, 17,        // Q W E R T
    0, 1, 2, 3, 5,             // A S D F G
    6, 7, 8, 9, 11,            // Z X C V B
    18, 19, 20, 21, 23,        // 1 2 3 4 5
    // Modifiers and controls
    56,                        // Left Shift
    59,                        // Left Control
    58,                        // Left Option
    55,                        // Left Command
    48,                        // Tab
    57,                        // Caps Lock
    // Function keys
    122, 120, 99, 118, 96, 97, // F1-F6
    // Other
    50,                        // ` ~
    53,                        // Escape
    49,                        // Space (assigned LEFT)
};

static const std::unordered_set<int> rightKeys = {
    16, 32, 34, 31, 35,          // Y U I O P
    4, 38, 40, 37,               // H J K L
    45, 46,                      // N M
    22, 26, 28, 25, 29,          // 6 7 8 9 0
    // Modifiers and controls
    60,                          // Right Shift
    62,                          // Right Control
    61,                          // Right Option
    54,                          // Right Command
    // Function keys
    98, 100, 101, 109, 103, 111, // F7-F12
    // Punctuation
    33, 30,                      // [ ]
    42,                          // Backslash
    41,                          // ;
    39,                          // '
    24,                          // =
    47,                          // .
    43,                          // ,
    44,                          // /
    // Navigation
    123, 124, 125, 126,          // Left Right Down Up arrows
    115, 119,                    // Home End
    116, 121,                    // Page Up Page Down
    117,                         // Forward Delete
    114,                         // Help / Insert-like key
    // Numpad
    82, 83, 84, 85, 86,          // 0-4
    87, 88, 89, 91, 92,          // 5-9
    65,                          // Decimal
    67, 69, 75, 78,              // * + / -
    71,                          // Clear / Num Lock equivalent
    76                           // Numpad Enter
};
static KeySide lastKeySide = KeySide::NONE;

void startGlobalInput(){
    [NSEvent addGlobalMonitorForEventsMatchingMask:(
        NSEventMaskKeyDown | NSEventMaskLeftMouseDown | NSEventMaskRightMouseDown |
        NSEventMaskOtherMouseDown | NSEventMaskKeyUp | NSEventMaskLeftMouseUp |
        NSEventMaskRightMouseUp | NSEventMaskOtherMouseUp | NSEventMaskFlagsChanged) handler:^(NSEvent *event){
        NSEventType type = [event type];
        unsigned short key = 0;
        if(type == NSEventTypeKeyDown || type == NSEventTypeKeyUp || type == NSEventTypeFlagsChanged){
            key = [event keyCode];
        }

        if(type == NSEventTypeKeyDown){
            //NSLog(@"GLOBAL KEY DOWN: %hu", [event keyCode]);

            auto result = heldKeys.insert(key);
            if(result.second){
                pressedKeys.insert(key);
                KeySide side = getKeySide(key);
                if(side != KeySide::NONE) lastKeySide = side;
            }

            if(key == 51 && result.second){
                backspacePressed = true;
            }
            else if(key != 51 && key != 36 && key != 27 && result.second){
                keyPressed = true;
            }
        }
        else if(type == NSEventTypeKeyUp){
            heldKeys.erase(key);
        }
        else if(type == NSEventTypeLeftMouseDown || type == NSEventTypeRightMouseDown || type == NSEventTypeOtherMouseDown){
            mousePressed = true;
            mouseDown = true;
        }
        else if(type == NSEventTypeLeftMouseUp || type == NSEventTypeRightMouseUp || type == NSEventTypeOtherMouseUp){
            mouseDown = false;
        }
        else if(type == NSEventTypeFlagsChanged && (key == 56 || key == 60)){
            auto flag = (key == 56) ? NX_DEVICELSHIFTKEYMASK : NX_DEVICERSHIFTKEYMASK;
            if([event modifierFlags] & flag) heldKeys.insert(key);
            else heldKeys.erase(key);
        }
    }];

    [NSEvent addLocalMonitorForEventsMatchingMask:(
        NSEventMaskKeyDown | NSEventMaskKeyUp | NSEventMaskLeftMouseDown |
        NSEventMaskLeftMouseUp | NSEventMaskRightMouseDown | NSEventMaskRightMouseUp |
        NSEventMaskOtherMouseDown | NSEventMaskOtherMouseUp | NSEventMaskFlagsChanged) handler:^NSEvent *(NSEvent *event){
        NSEventType type = [event type];
        unsigned short key = 0;
        if(type == NSEventTypeKeyDown || type == NSEventTypeKeyUp || type == NSEventTypeFlagsChanged){
            key = [event keyCode];
        }
        if(type == NSEventTypeKeyDown){
            auto result = heldKeys.insert(key);
            if(result.second){
                pressedKeys.insert(key);
                KeySide side = getKeySide(key);
                if(side != KeySide::NONE) lastKeySide = side;
            }
            if(key == 51 && result.second){
                backspacePressed = true;
            }
            else if(key != 51 && key != 36 && key != 27 && result.second){
                keyPressed = true;
            }
        }
        else if(type == NSEventTypeKeyUp){
            heldKeys.erase(key);
        }
        else if(type == NSEventTypeLeftMouseDown || type == NSEventTypeRightMouseDown || type == NSEventTypeOtherMouseDown){
            mousePressed = true;
            mouseDown = true;
        }
        else if(type == NSEventTypeLeftMouseUp || type == NSEventTypeRightMouseUp || type == NSEventTypeOtherMouseUp){
            mouseDown = false;
        }
        else if(type == NSEventTypeFlagsChanged && (key == 56 || key == 60)){
            auto flag = (key == 56) ? NX_DEVICELSHIFTKEYMASK : NX_DEVICERSHIFTKEYMASK;
            if([event modifierFlags] & flag) heldKeys.insert(key);
            else heldKeys.erase(key);
        }
        return event;
    }];
}

bool globalKeyPressed(){
    if(keyPressed){
        keyPressed = false;
        return true;
    }
    return false;
}

bool globalBackspacePressed(){
    if(backspacePressed){
        backspacePressed = false;
        return true;
    }
    return false;
}

bool globalMousePressed(){
    if(mousePressed){
        mousePressed = false;
        return true;
    }
    return false;
}

double globalMouseX(){
    NSPoint mouse = [NSEvent mouseLocation];
    return mouse.x;
}

double globalMouseY(){
    NSPoint mouse = [NSEvent mouseLocation];
    CGFloat screenHeight = [[NSScreen mainScreen] frame].size.height;
    return screenHeight - mouse.y;
}

bool globalMouseDown(){
    return mouseDown;
}

const char* resourcePath() {
    NSString *path = [[NSBundle mainBundle] resourcePath];
    return [path UTF8String];
}

void requestInputPermissions() {
    NSDictionary *options = @{
        (__bridge NSString *)kAXTrustedCheckOptionPrompt: @YES
    };

    AXIsProcessTrustedWithOptions(
        (__bridge CFDictionaryRef)options
    );

    CGRequestListenEventAccess();
    //NSLog(@"Input Monitoring: %d", CGPreflightListenEventAccess());
    //NSLog(@"Accessibility: %d", AXIsProcessTrusted());
}

static KeySide getKeySide(int keyCode){
    if(leftKeys.contains(keyCode)) return KeySide::LEFT;
    if(rightKeys.contains(keyCode)) return KeySide::RIGHT;
    return KeySide::NONE;
}

KeySide globalKeySidePressed(){
    return lastKeySide;
}

KeySide globalKeySideDown(){
    for(const auto& key : heldKeys){
        KeySide side = getKeySide(key);
        if(side != KeySide::NONE) return side;
    }
    return KeySide::NONE;
}

static int getKeyCode(const Key key){
    switch(key){
        case Key::ONE: return 18;
        case Key::HYPHEN: return 27;
        case Key::ENTER: return 36;
        case Key::BACKSPACE: return 51;
        case Key::SLASH: return 44;
        default: return -1;
    }
}

bool globalKeyPressed(Key key){
    if(key == Key::ANY && keyPressed){
        keyPressed = false;
        return true;
    }
    const int code = getKeyCode(key);
    if(code == -1) return false;
    return pressedKeys.erase(code) > 0;
}

bool globalKeyDown(const Key key){
    if(key == Key::ANY){
        return !heldKeys.empty();
    }
    if(key == Key::SHIFT){
        return heldKeys.contains(56) || heldKeys.contains(60);
    }

    const int code = getKeyCode(key);
    if(code == -1) return false;
    return heldKeys.contains(code);
}