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
void startGlobalInput(){
    [NSEvent addGlobalMonitorForEventsMatchingMask:
        (NSEventMaskKeyDown | NSEventMaskLeftMouseDown | NSEventMaskRightMouseDown |
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
            if(result.second) pressedKeys.insert(key);
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

    [NSEvent addLocalMonitorForEventsMatchingMask:
    (NSEventMaskKeyDown | NSEventMaskKeyUp | NSEventMaskLeftMouseDown |
     NSEventMaskLeftMouseUp | NSEventMaskRightMouseDown | NSEventMaskRightMouseUp |
     NSEventMaskOtherMouseDown | NSEventMaskOtherMouseUp | NSEventMaskFlagsChanged)
    handler:^NSEvent *(NSEvent *event){
        NSEventType type = [event type];
        unsigned short key = 0;
        if(type == NSEventTypeKeyDown || type == NSEventTypeKeyUp || type == NSEventTypeFlagsChanged){
            key = [event keyCode];
        }
        if(type == NSEventTypeKeyDown){
            auto result = heldKeys.insert(key);
            if(result.second) pressedKeys.insert(key);
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