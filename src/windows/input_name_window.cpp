#include "window_base.h"
/*Class inputNameWindow


*/
inputNameWindow::inputNameWindow(int userNumber) : userNumber(userNumber) {
    for (int i = 0; i < userNumber; ++i) {
        this->userNames.push_back(new char[1024]());
    }
};

inputNameWindow::~inputNameWindow() {
    for (auto i : this->userNames) {
        delete i;
    }
}

bool inputNameWindow::draw() {
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin(u8"输入用户名称", NULL,
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    for (int i = 0; i < this->userNumber; ++i) {
        ImGui::InputText(
            (std::string("user") + (std::to_string(i).c_str())).c_str(),
            this->userNames[i], 1024);
    }
    if (ImGui::Button("Entry")) {
        ImGui::End();
        return false;
    }
    ImGui::End();
    return true;
}
