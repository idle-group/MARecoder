#ifndef BACKENDS_H
#define BACKENDS_H
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
void setupBackends(GLFWwindow* window, const char* glsl_version);
void cleanupBackends();
#endif