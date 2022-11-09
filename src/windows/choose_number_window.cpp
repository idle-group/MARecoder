#include "imgui.h"
#include "window_base.h"
#include <iostream>
#include <string>
#include <vector>
/*Class chooseNumberWindow


*/
bool chooseNumberWindow::draw() {
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin(u8"choose 人数", NULL,
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    const char* items[] = {"3", "4", "5"};
    ImGui::Combo("用户人数", &this->chooseNum, items, 3);
    if (ImGui::Button("Entry")) {
        ImGui::End();
        return false;
    }
    ImGui::End();
    return true;
}

int chooseNumberWindow::getChooseNum() { return this->chooseNum + 3; }
