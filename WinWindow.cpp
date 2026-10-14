#include <windows.h>

void keepWindowTopmost(void* windowHandle) {
    HWND hwnd = static_cast<HWND>(windowHandle);

    SetWindowPos(
        hwnd,
        HWND_TOPMOST,
        0, 0, 0, 0,
        SWP_NOMOVE |
        SWP_NOSIZE |
        SWP_NOACTIVATE
    );
}