#ifndef BOTHGAME_H
#define BOTHGAME_H
#include"tool.h"
#include"ctrlpanel.h"
class BothGame : public tool
{
public:
    BothGame(QWidget*,bool side);
    void click(int id, int row, int col);   //重载点击函数
    bool is_over(int killed);               //重载判断结束函数
    void moveStone(int moveid, int row, int col) ;  //重载走棋函数

private:
    Ctrlpanel* panel;   //右半部控件
};

#endif // BOTHGAME_H
