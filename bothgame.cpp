#include "bothgame.h"

BothGame::BothGame(QWidget* parent,bool _side):tool(_side,parent)
{
    panel=new Ctrlpanel(1,this);//建立右半部控件
    /*建立悔棋，认输，超时的信号与槽*/
    connect(panel,&Ctrlpanel::_back,this,&tool::backOne);
    connect(panel,&Ctrlpanel::_loser,
            [=](){
            emit game_close(is_red,steps.size());
    });
    connect(panel,&Ctrlpanel::_overtime,
            [=](){
        panel->writeWindow(QString("%1超时,交换走棋。").arg(is_red?"红方":"黑方"));
        moveStone(0,s[0].row,s[0].col);
    });

    panel->Alltime_begin();//打开总时长计时器
    panel->perTime_begin();//打开每步剩余时长计时器
}

//点击函数
void BothGame::click(int id, int row, int col)
{
    tool::click(id,row,col);//调用父类函数
}

//判断结束函数
bool BothGame::is_over(int killed)
{
    if(killed==4||killed==20)
    {
        panel->perTime_end();//停止每步计时器
        panel->Alltime_end();//停止总时长计时器
        emit game_close(is_red,steps.size());//发送结束信号
        return true;
    }
    return false;
}

//走棋函数
void BothGame::moveStone(int moveid, int row, int col)
{
    tool::moveStone(moveid,row,col);//调用父类函数
    panel->setbuttonEnble(true);//打开悔棋，认输按钮
    panel->change_side(is_red);//改变行动方标签
    /*重置每步计时器*/
    panel->perTime_end();
    panel->perTime_begin();
}
