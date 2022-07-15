#ifndef CTRLPANEL_H
#define CTRLPANEL_H

#include <QWidget>
#include<QTimer>

namespace Ui {
class Ctrlpanel;
}

class Ctrlpanel : public QWidget
{
    Q_OBJECT

public:
    explicit Ctrlpanel(int game,QWidget *parent = nullptr);
    explicit Ctrlpanel();
    ~Ctrlpanel();

    void setbuttonEnble(bool enble);//设置悔棋，认输按钮状态
    void writeWindow(QString str);//写入系统框
    void writeChat(QString str,bool _side);//写入聊天框
    void setenble();//打开聊天发送按钮
    void Alltime_begin();//打开总计时期
    void Alltime_end();//关闭总计时期
    void perTime_begin();//打开每步计时器
    void perTime_end();//关闭每步计时器
    void change_side(bool _side);//换边显示

signals:
    void _back();//悔棋信号
    void _loser();//认输信号
    void _chat(QString str);//聊天信号
    void _overtime();//超时信号

private slots:
    /*按钮槽函数*/
    void on_pushButton_chat_released();

    void on_pushButton_window_released();

    void on_pushButton_send_released();

public:
    Ui::Ctrlpanel *panel_ui;
    QTimer *Alltime;//总计时器
    QTimer *per_time;//每步计时器
    int pertime;//每步最大时间
};

#endif // CTRLPANEL_H
