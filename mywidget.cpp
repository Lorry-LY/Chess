#include "mywidget.h"
#include "ui_mywidget.h"
#include<QMessageBox>
#include<QHBoxLayout>
#include<QIcon>
MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)

{
    //设置标题
    setWindowTitle("小宇象棋");

    //设置图标
    setWindowIcon(QIcon(":/new/image/logo.ico"));

    //设置窗口大小：最大最小相同，则不可放缩。
    setMaximumSize(1100,900);
    setMinimumSize(1100,900);

    //新建登录界面
    dia=new Dialog_login(this);
    //显示窗口（因为父窗口没有显示，所以需要自主显示）
    dia->show();
    /*设置接收游戏信号的信号与槽*/
    connect(dia,&Dialog_login::singel,this,&MyWidget::deal_singel);
    connect(dia,&Dialog_login::both,this,&MyWidget::deal_both);
    connect(dia,&Dialog_login::server,this,&MyWidget::deal_server);
    connect(dia,&Dialog_login::client,this,&MyWidget::deal_client);

}

MyWidget::~MyWidget()
{

}

//处理游戏结束槽函数
void MyWidget::deal_close(bool is_red,int number)
{
    QString str=QString("%1方胜利，总回合数：%2").arg(is_red?"黑":"红").arg(number/2);//组织弹出框内容
    QMessageBox::warning(this,"结束",str);//弹出框
    close();//关闭窗口
}

//处理人机对战信号槽函数
void MyWidget::deal_singel(int level,bool _back)
{
    dia->hide();//隐藏登录框
    singel=new singelGame(this,level,_back);//创建人机对战
    connect(singel,&tool::game_close,this,&MyWidget::deal_close);//建议游戏结束发送回主窗口的信号的链接
    move(400,50);//移动主窗口位置
    this->show();//显示主窗口
}

void MyWidget::deal_both()
{
    dia->hide();//隐藏登录框
    both=new BothGame(this,true);//创建人人对战
    connect(both,&tool::game_close,this,&MyWidget::deal_close);//建议游戏结束发送回主窗口的信号的链接
    move(400,50);//移动主窗口位置
    this->show();//显示主窗口
}

void MyWidget::deal_server()
{
    dia->hide();//隐藏登录框
    server=new serverGame(true,this);//创建服务器
    connect(server,&tool::game_close,this,&MyWidget::deal_close);//建议游戏结束发送回主窗口的信号的链接
    move(400,50);//移动主窗口位置
    this->show();//显示主窗口
}

void MyWidget::deal_client(QString ip,QString port)
{
    dia->hide();//隐藏登录框
    client=new clientGame(false,this,ip,port);//创建客户端
    connect(client,&tool::game_close,this,&MyWidget::deal_close);//建议游戏结束发送回主窗口的信号的链接
    move(400,50);//移动主窗口位置
    this->show();//显示主窗口
}
