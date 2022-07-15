#ifndef DIALOG_LOGIN_H
#define DIALOG_LOGIN_H

#include <QDialog>
#include<QPaintEvent>
namespace Ui {
class Dialog_login;
}

class Dialog_login : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_login(QWidget *parent = nullptr);
    ~Dialog_login();

signals:
    void singel(int level,bool side);   //发送人机对战信号
    void both();                        //发送人人对战信号
    void server();                      //发送服务器对战信号
    void client(QString ip,QString port);   //发送客户端对战信号

private slots:
    /*按钮槽函数*/
    void on_pushButton_back2_released();

    void on_pushButton_back1_released();

    void on_pushButton_back_released();

    void on_pushButton_per_rob_released();

    void on_pushButton_per_cilent_released();

    void on_pushButton_per_per_released();

    void on_pushButton_per_server_released();

    void on_pushButton_open1_released();

    void on_pushButton_open2_released();

private:
    Ui::Dialog_login *ui;
};

#endif // DIALOG_LOGIN_H
