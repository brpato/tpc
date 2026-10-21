#import <AppKit/AppKit.h>
#import <ApplicationServices/ApplicationServices.h>
#include "GlobalInput.h"
#include <unordered_set>

static bool keyPressed = false;
static bool backspacePressed = false;
static bool mousePressed = false;
static std::unordered_set<unsigned short> heldKeys;
static bool mouseDown = false;
void startGlobalInput(){
    [NSEvent addGlobalMonitorForEventsMatchingMask:
        (NSEventMaskKeyDown | NSEventMaskLeftMouseDown | NSEventMaskRightMouseDown |
         NSEventMaskOtherMouseDown | NSEventMaskKeyUp | NSEventMaskLeftMouseUp |
         NSEventMaskRightMouseUp | NSEventMaskOtherMouseUp) handler:^(NSEvent *event){
        if([event type] == NSEventTypeKeyDown){
            NSLog(@"GLOBAL KEY DOWN: %hu", [event keyCode]);

            unsigned short key = [event keyCode];
            auto result = heldKeys.insert(key);

            if(key == 51){
                backspacePressed = true;
            }
            else if(result.second){
                keyPressed = true;
            }
        }
        else if([event type] == NSEventTypeKeyUp){
            unsigned short key = [event keyCode];
            heldKeys.erase(key);
        }
        else if([event type] == NSEventTypeLeftMouseDown || [event type] == NSEventTypeRightMouseDown || [event type] == NSEventTypeOtherMouseDown){
            mousePressed = true;
            mouseDown = true;
        }
        else if([event type] == NSEventTypeLeftMouseUp || [event type] == NSEventTypeRightMouseUp || [event type] == NSEventTypeOtherMouseUp){
            mouseDown = false;
        }
    }];

    [NSEvent addLocalMonitorForEventsMatchingMask:
    (NSEventMaskKeyDown | NSEventMaskKeyUp | NSEventMaskLeftMouseDown |
     NSEventMaskLeftMouseUp | NSEventMaskRightMouseDown | NSEventMaskRightMouseUp |
     NSEventMaskOtherMouseDown | NSEventMaskOtherMouseUp)
    handler:^NSEvent *(NSEvent *event){
        if ([event type] == NSEventTypeKeyDown){
            unsigned short key = [event keyCode];
            auto result = heldKeys.insert(key);

            if (key == 51){
                backspacePressed = true;
            }
            else if(result.second){
                keyPressed = true;
            }
        }
        else if ([event type] == NSEventTypeKeyUp){
            unsigned short key = [event keyCode];
            heldKeys.erase(key);
        }
        else if([event type] == NSEventTypeLeftMouseDown || [event type] == NSEventTypeRightMouseDown || [event type] == NSEventTypeOtherMouseDown){
            mousePressed = true;
            mouseDown = true;
        }
        else if([event type] == NSEventTypeLeftMouseUp || [event type] == NSEventTypeRightMouseUp || [event type] == NSEventTypeOtherMouseUp){
            mouseDown = false;
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

    NSLog(@"Input Monitoring: %d", CGPreflightListenEventAccess());
    NSLog(@"Accessibility: %d", AXIsProcessTrusted());
}

static int getKeyCode(const Key key){
    switch(key){
        case Key::BACKSPACE: return 51;
        default: return -1;
    }
}

bool globalKeyDown(const Key key){
    if(key == Key::ANY){
        return !heldKeys.empty();
    }

    const int code = getKeyCode(key);
    if(code == -1) return false;
    return heldKeys.contains(code);
}