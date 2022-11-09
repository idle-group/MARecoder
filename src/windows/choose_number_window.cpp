#include "imgui.h"
#include "windows/window_base.h"
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

/*Class recoderWindow


*/
recorderWindow::recorderWindow(inputNameWindow* input)
    : userNumber(input->userNumber), cardTable(5, std::vector<int>(4, 0)),
      userTable(input->userNumber, std::vector<int>{100, 0}) {
    for (auto it = input->userNames.begin(); it != (input->userNames.end());
         ++it) {
        this->userNames.push_back(*it);
        *it = nullptr;
    }
}

bool recorderWindow::draw() {
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin(u8"MARecorder", NULL,
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    this->drwaCardTable();
    ImGui::SameLine();
    this->drawUserTable();
    ImGui::Separator();
    this->drawAuctionAct();
    ImGui::End();

    return true;
}

void recorderWindow::drwaCardTable() {
    static const ImGuiTableFlags flags =
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
        ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit;
    ;
    if (ImGui::BeginTable("table1", 7, flags)) {

        // 表头
        {
            ImGui::TableSetupColumn(u8"颜色");
            ImGui::TableSetupColumn("1");
            ImGui::TableSetupColumn("2");
            ImGui::TableSetupColumn("3");
            ImGui::TableSetupColumn("4");
            ImGui::TableSetupColumn(u8"总计");
            ImGui::TableSetupColumn(u8"剩余");
            ImGui::TableHeadersRow();
        }

        std::vector<const char*> color{u8"黄", u8"蓝", u8"红", u8"绿", u8"橙"};
        for (int row = 0; row < 5; row++) {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            char buf[32];
            sprintf(buf, "%s", color[row]);
            ImGui::TextUnformatted(buf);

            for (int column = 1; column < 5; column++) {
                ImGui::TableSetColumnIndex(column);
                char buf[32];
                sprintf(buf, "%d", this->cardTable[row][column - 1]);
                if (this->cardTable[row][column - 1] != 0) {
                    std::cout << row << ":" << column - 1 << std::endl;
                }
                ImGui::TextUnformatted(buf);
            }
            ImGui::TableSetColumnIndex(5);
            sprintf(buf, "%d", 8);
            ImGui::TextUnformatted(buf);
            ImGui::TableSetColumnIndex(6);
            sprintf(buf, "%d", 4);
            ImGui::TextUnformatted(buf);
        }

        ImGui::EndTable();
    }
}

void recorderWindow::drawUserTable() {
    static const ImGuiTableFlags flags = ImGuiTableFlags_Borders |
                                         ImGuiTableFlags_RowBg |
                                         ImGuiTableFlags_NoHostExtendX;

    if (ImGui::BeginTable("userTable", 3, flags)) {

        // 表头
        {
            ImGui::TableSetupColumn(u8"用户");
            ImGui::TableSetupColumn(u8"资金");
            ImGui::TableSetupColumn(u8"手牌数");
            ImGui::TableHeadersRow();
        }

        for (int row = 0; row < this->userNumber; row++) {
            ImGui::TableNextRow();
            // 用户名
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", this->userNames[row]);

            // 金额+手牌数
            for (int column = 1; column < 3; column++) {
                ImGui::TableSetColumnIndex(column);
                char buf[32];
                sprintf(buf, "%d", this->userTable[row][column - 1]);
                ImGui::TextUnformatted(buf);
            }
        }
        ImGui::EndTable();
    }
}

void recorderWindow::drawAuctionAct() {
    ImGui::Text("当前玩家：%s", this->userNames[this->currentUser]);
    static int chooseNum = 0;
    const char* auctionType[] = {u8"秘密", u8"公开", u8"依次", u8"一口价",
                                 u8"联合"};
    ImGui::Text("排卖种类");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetIO().DisplaySize.x * 0.1);
    ImGui::Combo("##拍卖种类", &chooseNum, auctionType, 5);
    ImGui::SameLine();
    static int color = 0;
    ImGui::Text("牌的颜色");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetIO().DisplaySize.x * 0.08);
    ImGui::Combo("##牌的颜色", &color, "黄\0蓝\0红\0绿\0橙\0\0");
    // ImGui::SameLine();
    if (chooseNum == 4) {
        std::string userNames;
        static int NextUserChooseNum = 0;
        for (auto name : this->userNames) {
            userNames.append(name);
            userNames.push_back('\0');
        }
        userNames.push_back('\0');
        ImGui::Text("联合玩家：");
        // ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetIO().DisplaySize.x * 0.3);
        ImGui::Combo("##联合玩家", &NextUserChooseNum, userNames.c_str());
    }
    ImGui::SameLine();
    static int price = 0;
    ImGui::SetNextItemWidth(ImGui::GetIO().DisplaySize.x * 0.2);
    ImGui::InputInt("##input int", &price);
    if (ImGui::Button("done")) {
        

    }
}