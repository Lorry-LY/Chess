#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include"bothgame.h"
#include"singelgame.h"
#include"servergame.h"
#include"clientgame.h"



class MyWidget : public QWidget
{


public:
    MyWidget(QWidget *parent = nullptr);
    ~MyWidget();

public slots:
    void deal_close(bool is_red,int number);    //处理结束信号槽函数
    void deal_singel(int level,bool _back);     //处理人机对战槽函数
    void deal_both();                           //处理人人对战槽函数
    void deal_server();                         //处理服务器槽函数
    void deal_client(QString ip,QString port);  //处理客户端槽函数
private:
    Dialog_login* dia;      //选择游戏模式窗口
    singelGame* singel;     //人机对战
    BothGame* both;         //人人对战
    serverGame* server;     //联网对战服务器
    clientGame* client;     //联网对战客户端

};
#endif // MYWIDGET_H
