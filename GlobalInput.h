// GlobalInput.h
#pragma once

void startGlobalInput();
bool globalMousePressed();
double globalMouseX();
double globalMouseY();
bool globalMouseDown();
const char* resourcePath();
void requestInputPermissions();

enum class KeySide{
    LEFT, RIGHT, NONE
};
KeySide globalKeySidePressed();
KeySide globalKeySideDown();

enum class Key{
    ANY, SHIFT, ONE, HYPHEN, ENTER, BACKSPACE, SLASH
};
bool globalKeyPressed(Key key);
bool globalKeyDown(Key key);

