#include "imgui.h"
#include "window_base.h"
#include <string>
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
    ImGui::Text("拍卖种类");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetIO().DisplaySize.x * 0.1);
    ImGui::Combo("##拍卖种类", &chooseNum, auctionType, 5);
    ImGui::SameLine();
    static int color = 0;
    ImGui::Text("牌的颜色");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetIO().DisplaySize.x * 0.08);
    ImGui::Combo("##牌的颜色", &color, "黄\0蓝\0红\0绿\0橙\0\0");
    static int NextUserChooseNum = 0;
    if (chooseNum == 4) {
        std::string userNames;
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
    static int toUser = -1;
    ImGui::Text("购买玩家");
    ImGui::SameLine();

    {
        std::string userNames;
        for (auto name : this->userNames) {
            userNames.append(name);
            userNames.push_back('\0');
        }
        userNames.append("none");
        userNames.push_back('\0');
        userNames.push_back('\0');
        ImGui::SetNextItemWidth(ImGui::GetIO().DisplaySize.x * 0.3);
        ImGui::Combo("##购买玩家", &toUser, userNames.c_str());
    }

    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetIO().DisplaySize.x * 0.2);
    if (ImGui::Button("iiint nextUsernt nextUsernt nextUserdone")) {
        if (toUser == this->userNumber) {
            toUser = -1;
        }
        switch (chooseNum) {
        case 0:
            this->secretAuc(this->currentUser, toUser, color, price);
            break;
        case 1:
            this->openAuc(this->currentUser, toUser, color, price);
            break;
        case 2:
            this->orderAuc(this->currentUser, toUser, color, price);
            break;
        case 3:
            this->onceAuc(this->currentUser, toUser, color, price);
            break;
        case 4:
            this->unionAuc(this->currentUser, NextUserChooseNum, toUser, color,
                           price);
        }
        // 判断有没有五张
        if(this->cardTable[color][currentRound]>=5){
            currentRound+=1;
            if(currentRound>=4) {
                // Gameover
                this->isDisplay = false;
            }
        }

    }
}

void recorderWindow::secretAuc(int fromUser, int toUser, int color, int price) {
    // 修改卡牌统计
    this->cardTable[color][currentRound] += 1;
    // 修改金钱
    this->userTable[toUser][0] -= price;
    if (toUser != -1)
        this->userTable[fromUser][0] += price;
    // 修改手中手牌数
    this->userTable[fromUser][1] -= 1;
    // 设定当前用户
    this->currentUser = (fromUser + 1) % this->userNumber;
}

void recorderWindow::openAuc(int fromUser, int toUser, int color, int price) {
    this->secretAuc(fromUser, toUser, color, price);
}

void recorderWindow::orderAuc(int fromUser, int toUser, int color, int price) {
    this->secretAuc(fromUser, toUser, color, price);
}

void recorderWindow::unionAuc(int fromUser, int nextUser, int toUser, int color,
                              int price) {
    // 没有人跟 卡直接给自己
    if (nextUser == -1) {
        // 修改卡牌统计
        this->cardTable[color][currentRound] += 1;
        // 修改手中卡牌数
        this->userTable[fromUser][1] -= 1;
        // 修改当前拍卖玩家
        this->currentUser = (fromUser + 1) % this->userNumber;
    } else {
        // 修改卡牌统计
        this->cardTable[color][currentRound] += 2;
        // 修改金钱
        this->userTable[nextUser][0] += price;
        this->userTable[toUser][0] -= price;
        // 修改手中的卡牌数
        this->userTable[fromUser][1] -= 1;
        this->userTable[nextUser][1] -= 1;
        // 修改当前的拍卖玩家
        this->currentUser = (nextUser + 1) % this->userNumber;
    }
}

void recorderWindow::dealCards(){
    
}