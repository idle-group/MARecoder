#ifndef WINDOW_BASE_H
#define WINDOW_BASE_H
#include "imgui.h"
#include <locale>
#include <vector>
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
    std::vector<std::vector<int>> userTable;

  public:
    recorderWindow(inputNameWindow* input);
    bool draw();
    void drwaCardTable();
    void drawUserTable();
    void drawAuctionAct();

    // 五种拍卖方式的处理函数
    
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