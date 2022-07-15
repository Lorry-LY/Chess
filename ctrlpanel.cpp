#include "ctrlpanel.h"
#include "ui_ctrlpanel.h"
#include<QDateTime>


Ctrlpanel::Ctrlpanel(int game,QWidget *parent) :
    QWidget(parent),
    panel_ui(new Ui::Ctrlpanel)
{
    panel_ui->setupUi(this);
    /*初始化按钮状态和样式*/
    panel_ui->pushButton_back->setEnabled(false);
    panel_ui->pushButton_loser->setEnabled(false);
    panel_ui->pushButton_send->setEnabled(false);
    panel_ui->pushButton_chat->setFlat(true);
    panel_ui->pushButton_window->setFlat(true);
    /*初始化页面*/
    panel_ui->stackedWidget_2->setCurrentIndex(1);
    /*建立信号与槽*/
    connect(panel_ui->pushButton_back,&QPushButton::released,
            [=]()
    {
        emit _back();
    }
    );
    connect(panel_ui->pushButton_loser,&QPushButton::released,
            [=]()
    {
        emit _loser();
    }
    );
    /*建立计时器*/
    Alltime=new QTimer(this);
    per_time=new QTimer(this);
    /*建立计时器槽函数*/
    connect(Alltime,&QTimer::timeout,
            [=]()
    {
        static int i=0;
        i++;
        panel_ui->label_times->setText(QString("%1:%2")
                                       .arg(i/60,2,10,QLatin1Char('0'))     //分钟，不足两位左填充0
                                       .arg(i%60,2,10,QLatin1Char('0')));   //秒钟。
    });
    connect(per_time,&QTimer::timeout,
            [=]()
    {
        pertime--;
        panel_ui->label_onlytime->setText(QString("%1s").arg(pertime+1));
        if(pertime<0)
        {
            perTime_end();
            emit _overtime();
        }

    });

    /*根据父对象初始化窗口*/
    if(game==1||game==2)
    {
        panel_ui->stackedWidget->setCurrentIndex(0);
    }
    else if(game==3||game==4)
    {
        panel_ui->stackedWidget->setCurrentIndex(1);
    }
}

void Ctrlpanel::change_side(bool _side)
{
    //改变行动方
    panel_ui->label_action->setText(QString("%1").arg(_side?"红方":"黑方"));
}

void Ctrlpanel::Alltime_begin()
{
    /*计时器没有工作的情况下打开计时器*/
    if(Alltime->isActive()==false)
    {
        Alltime->start(1000);
    }
}

void Ctrlpanel::Alltime_end()
{
    /*计时器工作的情况下关闭计时器*/
    if(Alltime->isActive()==true)
    {
        Alltime->stop();
    }
}

void Ctrlpanel::perTime_begin()
{
    pertime=120;//设置最大时间
    /*计时器没有工作的情况下打开计时器*/
    if(per_time->isActive()==false)
    {
        per_time->start(1000);
    }
}

void Ctrlpanel::perTime_end()
{
    /*计时器工作的情况下关闭计时器*/
    if(per_time->isActive()==true)
    {
        per_time->stop();
    }
}

void Ctrlpanel::setenble()
{
    //打开聊天框的发送按钮
    panel_ui->pushButton_send->setEnabled(true);
}

Ctrlpanel::~Ctrlpanel()
{
    delete panel_ui;
}

void Ctrlpanel::setbuttonEnble(bool enble)
{
    //设置悔棋、认输按钮
    panel_ui->pushButton_back->setEnabled(enble);
    panel_ui->pushButton_loser->setEnabled(enble);
}



void Ctrlpanel::writeWindow(QString str)
{
    QString info=QDateTime::currentDateTime().toString()+"\n"+str;//获取当前时间
    panel_ui->textEdit_window->append(info);
}

void Ctrlpanel::writeChat(QString str,bool _side)
{
    QString turn=_side?"我：":"敌方：";
    QString info=QDateTime::currentDateTime().toString()+'\n'+turn+str;
    panel_ui->textEdit_read->append(info);
}

/*内部按钮切换界面*/
void Ctrlpanel::on_pushButton_chat_released()
{
    panel_ui->stackedWidget_2->setCurrentIndex(0);
}
void Ctrlpanel::on_pushButton_window_released()
{
    panel_ui->stackedWidget_2->setCurrentIndex(1);
}

/*发送按钮*/
void Ctrlpanel::on_pushButton_send_released()
{
    QString str=panel_ui->textEdit_write->toPlainText().toUtf8().data();//获取输入框信息
    panel_ui->textEdit_write->clear();//清空输入框
    emit _chat(str);//发送聊天信号
}
