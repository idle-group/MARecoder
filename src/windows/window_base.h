#ifndef WINDOW_BASE_H
#define WINDOW_BASE_H
#include "imgui.h"
#include <locale>
#include <sys/types.h>
#include <vector>
#include <array>
class windowBase {
  public:
    windowBase() = default;
    ~windowBase() = default;
    bool isDisplay = true;
    virtual bool draw() = 0;
};
class chooseNumberWindow : public windowBase {
    int chooseNum = 0;

  public:
    bool draw();
    int getChooseNum();
};

class inputNameWindow;

class recorderWindow : public windowBase {
    int userNumber;
    std::vector<char*> userNames;
    // 当前用户和当前轮次
    int currentUser = 0;
    int currentRound = 0;

    // 橙 绿 红 蓝 黄
    std::vector<std::vector<int>> cardTable;
    std::vector<std::vector<int>> userTable; // userTable[user][0] 资金 userTable[user][1] 手牌数 

    // 各位玩家在每个轮次的买到的牌
    std::vector<std::array<int,4>> userOwn;
  public:
    recorderWindow(inputNameWindow* input);
    bool draw();
    void drwaCardTable();
    void drawUserTable();
    void drawAuctionAct();

    // 五种拍卖方式的处理函数
    // 秘密拍卖
    void secretAuc(int fromUser, int toUser,int color,int price);
    // 公开拍卖
    void openAuc(int fromUser,int toUser,int color,int price);
    // 依次拍卖
    void orderAuc(int fromUser,int toUser,int color,int price);
    // 一口价拍卖
    void onceAuc(int fromUser, int toUser, int color, int price);
    // 联合拍卖
    void unionAuc(int fromUser,int nextUser,int toUser,int color,int price);

    // cardTable 更新
    void updateCardTable();
    // 发牌
    void dealCards();
    // isOverRound 判断这一轮是否结束
    bool isOverRound();
    // isOverGame 
    bool isOverGame();
    bool isOverGame(int round);
    
};

class inputNameWindow : public windowBase {
    int userNumber;
    std::vector<char*> userNames;

  public:
    inputNameWindow(int userNumber);
    ~inputNameWindow();
    friend recorderWindow::recorderWindow(inputNameWindow* input);
    bool draw();
};

#endif