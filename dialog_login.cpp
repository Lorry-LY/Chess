#include "dialog_login.h"
#include "ui_dialog_login.h"

Dialog_login::Dialog_login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_login)
{
    ui->setupUi(this);

    //设置标题
    setWindowTitle("小宇象棋——登录界面");

    //设置默认按钮
    ui->pushButton_per_rob->setFocus();

    /*设置按钮样式*/
    ui->pushButton_back->setFlat(true);
    ui->pushButton_back1->setFlat(true);
    ui->pushButton_back2->setFlat(true);
    ui->pushButton_open1->setFlat(true);
    ui->pushButton_open2->setFlat(true);
    ui->pushButton_per_per->setFlat(true);
    ui->pushButton_per_rob->setFlat(true);
    ui->pushButton_per_cilent->setFlat(true);
    ui->pushButton_per_server->setFlat(true);
}

Dialog_login::~Dialog_login()
{
    delete ui;
}

//返回按钮，返回到首页
void Dialog_login::on_pushButton_back2_released()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void Dialog_login::on_pushButton_back1_released()
{
    ui->stackedWidget->setCurrentIndex(0);
}

//退出按钮，退出程序
void Dialog_login::on_pushButton_back_released()
{
    close();
}

//人机对战按钮，切换到具体选项界面
void Dialog_login::on_pushButton_per_rob_released()
{
    ui->stackedWidget->setCurrentIndex(1);
}

//客户端对战按钮，切换到ip,port入口
void Dialog_login::on_pushButton_per_cilent_released()
{
    ui->stackedWidget->setCurrentIndex(2);
}

//人人对战按钮，发送信号
void Dialog_login::on_pushButton_per_per_released()
{
    emit both();
}

//服务器按钮，发送服务器信号
void Dialog_login::on_pushButton_per_server_released()
{
    emit server();
}

//人机对战开始游戏按钮
void Dialog_login::on_pushButton_open1_released()
{
    int level=ui->comboBox->currentIndex(); //获取选择的难度
    emit singel(level,true);    //发送信号，并传参；
}

//客户端开始游戏按钮
void Dialog_login::on_pushButton_open2_released()
{
    QString ip=ui->lineEdit_ip->text().toUtf8().data(); //获取IP
    QString port=ui->lineEdit_port->text().toUtf8().data(); //获取端口
    emit client(ip,port);   //发送信号，并传参
}
