#ifndef INIT_WINDOW_H
#define INIT_WINDOW_H
#include "imgui.h"
#include <GLFW/glfw3.h> // Will grag system OPenGL headers

const char* decideGLFW();

GLFWwindow* createMainWindow();
void setupMainWindowImGui();
#endif