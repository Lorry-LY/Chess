#ifndef CLIENTGAME_H
#define CLIENTGAME_H
#include"tool.h"
#include"ctrlpanel.h"
#include<QTcpServer>
#include<QTcpSocket>
class clientGame : public tool
{
public:
    clientGame(bool side,QWidget *parent = nullptr,QString ip="127.0.0.1",QString port="8888");

    void back();//悔棋
    void loser();//认输
    void send_chat(QString str);//聊天
    void click(int id, int row, int col);//选棋
    bool is_over(int killed);//重置判断结束
    bool true_over(bool s4,bool s20);//真正判断结束的函数
    void moveStone(int moveid, int row, int col) ;//走棋

    void backFromNetwork(QByteArray buf);//对方端悔棋
    void clickFromNetwork(QByteArray buf);//对方端选棋
    void initFromNetwork(QByteArray buf);//对方端初始化
    void loserFromNetwork(QByteArray buf);//对方端认输
    void chatFromNetwork(QByteArray buf);//对方发消息

    void slotDataArrive();  //数据传递槽函数

private:
    QTcpSocket *tcpSocket;//客户端
    Ctrlpanel *panel;//右控件
};

#endif // CLIENTGAME_H
