#ifndef NETGAME_H
#define NETGAME_H
#include"tool.h"
#include"ctrlpanel.h"
#include<QTcpServer>
#include<QTcpSocket>
#include<QMessageBox>
class netGame : public tool
{
public:
    netGame(QWidget*,bool server,QString ip="127.0.0.1",QString port="8888");
    ~netGame();
    bool is_server;//判断是不是服务器
    QTcpServer* server;//服务器
    QTcpSocket* socket;//客户端

    void back();//悔棋
    void click(int id, int row, int col);//选棋
    bool is_over(int killed);//重置判断结束
    bool true_over(bool s4,bool s20);//真正判断结束的函数
    void moveStone(int moveid, int row, int col) ;//走棋

    void backFromNetwork(QByteArray buf);//对方端悔棋
    void clickFromNetwork(QByteArray buf);//对方端选棋
    void initFromNetwork(QByteArray buf);//对方端初始化
    void loserFromNetwork(QByteArray buf);//对方端认输

signals:
    void game_over();

public slots:
    void slotNewConnection(); //链接服务器槽函数
    void slotDataArrive();  //数据传递槽函数

private:
    Ctrlpanel* panel;


};

#endif // NETGAME_H
