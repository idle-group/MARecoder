#include "backends.h"
#include "imgui.h"
#include "init_window.h"
#include "windows/window_base.h"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <iterator>
static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "Glfw Erroe %d: %s\n", error, description);
}

void drawWindow(GLFWwindow* window, windowBase* mainWindow) {
    // 接受glfw事件
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    if (!mainWindow->draw()) {
        mainWindow->isDisplay = false;
    }
    // Rendering
    ImGui::Render();
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // swap buffer
    glfwSwapBuffers(window);
}

/*主函数*/
int main() {
    //设定glfw错误回调
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        return 1;
    }
    const char* version = decideGLFW();
    setupMainWindowImGui();
    GLFWwindow* window = createMainWindow();
    setupBackends(window, version);

    // choose window
    chooseNumberWindow* chooseWindow = new chooseNumberWindow();
    while (!glfwWindowShouldClose(window) && chooseWindow->isDisplay) {
        drawWindow(window, chooseWindow);
    }
    int userNumber = chooseWindow->getChooseNum();
    delete chooseWindow;

    // imput Name
    inputNameWindow* inputWindow = new inputNameWindow(userNumber);
    while (!glfwWindowShouldClose(window) && inputWindow->isDisplay) {
        drawWindow(window, inputWindow);
    }

    // resize 
    glfwSetWindowSize(window, 640, 360);

    // Recorder window
    recorderWindow* recoder = new recorderWindow(inputWindow);

    while (!glfwWindowShouldClose(window) && recoder->isDisplay) {
        drawWindow(window, recoder);
    }

    
    cleanupBackends();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}