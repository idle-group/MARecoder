#include "imgui.h"
#include "window_base.h"
#include <array>
#include <map>
#include <numeric>
#include <string>
/*Class recoderWindow


*/
recorderWindow::recorderWindow(inputNameWindow* input)
    : userNumber(input->userNumber), cardTable(5, std::vector<int>(6, 0)),
      userTable(input->userNumber, std::vector<int>{100, 0}),userOwn(input->userNumber,std::array<int, 4>{}) {
    // 拷贝构造
    for (auto it = input->userNames.begin(); it != (input->userNames.end());
         ++it) {
        this->userNames.push_back(*it);
        *it = nullptr;
    }

    // init card 就是第一把发牌
    this->dealCards();
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
            // 总计
            ImGui::TableSetColumnIndex(5);
            sprintf(buf, "%d", this->cardTable[row][4]);
            ImGui::TextUnformatted(buf);
            // 剩余
            ImGui::TableSetColumnIndex(6);
            sprintf(buf, "%d", this->cardTable[row][5]);
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
    if (ImGui::Button("done")) {
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
            break;
        }

        // 判断这一轮次是否结束
        if (this->isOverRound()) {
            // 本轮结束 游戏轮次增加
            this->currentRound += 1;
            // 当前游戏没有结束的话
            if (!this->isOverGame()) {
                // 发牌正常进入下一轮
                this->dealCards();
            } else {
                // 运行到这里说明游戏结束拉
                this->isDisplay = false;
            }
        }
    }
}
// toUser=-1说明从银行那里买的
void recorderWindow::secretAuc(int fromUser, int toUser, int color, int price) {
    // 修改卡牌统计
    this->cardTable[color][currentRound] += 1;
    // 修改手中手牌数
    this->userTable[fromUser][1] -= 1;
    // 设定当前用户
    this->currentUser = (fromUser + 1) % this->userNumber;

    // 修改金钱 // 金钱是特别的
    // 轮次的最后一张牌不会涉足交易那么金钱的变化不会发生
    // 对于卡牌表的更新(这里只涉及对于表最后两列的计算问题)
    this->updateCardTable();
    if (!this->isOverRound()) {
        if (toUser != -1) {
            this->userTable[toUser][0] -= price;
            this->userTable[fromUser][0] += price;
        }else {
            this->userTable[fromUser][0]-=price;
        }
    }
}

void recorderWindow::openAuc(int fromUser, int toUser, int color, int price) {
    this->secretAuc(fromUser, toUser, color, price);
}

void recorderWindow::orderAuc(int fromUser, int toUser, int color, int price) {
    this->secretAuc(fromUser, toUser, color, price);
}

void recorderWindow::onceAuc(int fromUser, int toUser, int color, int price) {
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
        // 修改手中的卡牌数
        this->userTable[fromUser][1] -= 1;
        this->userTable[nextUser][1] -= 1;
        // 修改当前的拍卖玩家
        this->currentUser = (nextUser + 1) % this->userNumber;
        if (!this->isOverRound()) {
            // 修改金钱
            this->userTable[nextUser][0] += price;
            this->userTable[toUser][0] -= price;
        }
    }
}

void recorderWindow::updateCardTable() {
    // 对所有颜色进行更新
    for (int i = 0; i < 5; i++) {
        this->cardTable[i][4] = std::accumulate(
            this->cardTable[i].begin(), this->cardTable[i].end() - 2, 0);
    }
}

void recorderWindow::dealCards() {
    std::map<int, std::array<int, 4>> cardNum{
        {3, {10, 6, 6, 0}}, {4, {9, 4, 4, 0}}, {5, {8, 3, 3, 0}}};
    for (int i = 0; i < this->userNumber; ++i) {
        this->userTable[i][1] += cardNum[this->userNumber][this->currentRound];
    }
}

bool recorderWindow::isOverRound() {
    // 一轮结束有以下条件
    // * 某个花色的数量达到5
    // * 某位玩家失去所有手牌

    // 对于花色的判断
    for (int i = 0; i < 5; ++i) {
        if (this->cardTable[i][this->currentRound] >= 5) {
            return true;
        }
    }

    // 对于玩家手牌数量的判断
    for (int i = 0; i < this->userNumber; ++i) {
        if (this->userTable[i][1] <= 0) {
            return true;
        }
    }
    return false;
}

bool recorderWindow::isOverGame() {
    // 游戏结束有两个条件
    // *游戏第四轮结束
    if (this->currentRound == 5) {
        return true;
    }
    return false;
}

bool recorderWindow::isOverGame(int round) {
    if (round) {
        return true;
    }
    return false;
}
