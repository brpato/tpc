// GlobalInput.h
#pragma once

void startGlobalInput();
bool globalKeyPressed();
bool globalBackspacePressed();
bool globalMousePressed();
double globalMouseX();
double globalMouseY();
bool globalKeyDown();
bool globalMouseDown();
const char* resourcePath();
void requestInputPermissions();