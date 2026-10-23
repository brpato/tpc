// GlobalInput.h
#pragma once

void startGlobalInput();
bool globalKeyPressed();
bool globalBackspacePressed();
bool globalMousePressed();
double globalMouseX();
double globalMouseY();
bool globalMouseDown();
const char* resourcePath();
void requestInputPermissions();

enum class Key{
    ANY, SHIFT, ONE, HYPHEN, ENTER, BACKSPACE, SLASH
};

enum class KeySide{
    LEFT, RIGHT, NONE
};
bool globalKeyPressed(Key key);
bool globalKeyDown(Key key);