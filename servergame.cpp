#include "servergame.h"

#include<QMessageBox>
serverGame::serverGame(bool _side,QWidget *parent):tool(_side,parent)
{

    tcpServer=NULL;
    tcpSocket=NULL;
    /*创建右控件*/
    panel=new Ctrlpanel(3,this);
    connect(panel,&Ctrlpanel::_back,this,&serverGame::back);//建立悔棋信号与槽
    connect(panel,&Ctrlpanel::_loser,this,&serverGame::loser);//建议认输信号与槽
    connect(panel,&Ctrlpanel::_chat,this,&serverGame::send_chat);//建议聊天信号与槽
    connect(panel,&Ctrlpanel::_overtime,                    //建议超时信号与槽
            [=](){
        panel->writeWindow(QString("%1超时,交换走棋。").arg(is_red?"红方":"黑方"));
        moveStone(0,s[0].row,s[0].col);
    });

    panel->Alltime_begin();//打开总计时器
    panel->perTime_begin();//打开每步计时器


    tcpServer=new QTcpServer(this);//建立服务器

    tcpServer->listen(QHostAddress::Any,1314);//服务器监听

    QString temp=QString ("正在等待客户端连接……");
    panel->writeWindow(temp);//在系统窗口写入信息
    //建议新链接信号与槽
    connect(tcpServer,&QTcpServer::newConnection,
            [=]()
    {
        //取出建立好链接的套接字
        tcpSocket = tcpServer->nextPendingConnection();
        //获取对方的IP和端口
        QString ip=tcpSocket->peerAddress().toString();
        qint16 port=tcpSocket->peerPort();
        QString temp=QString ("客户端[%1:%2]::成功链接").arg(ip).arg(port);
        panel->writeWindow(temp);//写入系统窗口
        panel->setenble();//打开聊天发送信息按钮
        //建立监听信号与槽
        connect(tcpSocket,&QTcpSocket::readyRead,this,&serverGame::slotDataArrive);
    }
    );
}

void serverGame::back()
{
    if(is_red != side)
    {
        return;//不到自己出牌，不能悔棋
    }
    //悔两步棋
    backOne();
    backOne();

    QByteArray buf;
    buf.append(3);//3代表悔棋指令
    tcpSocket->write(buf);//发送指令
    panel->writeWindow(QString("%1悔棋。").arg(is_red?"红方":"黑方"));//写入悔棋信息
}

void serverGame::loser()
{
    //结束计时器
    panel->Alltime_end();
    panel->perTime_end();
    /*发送认输消息*/
    QByteArray buf;
    buf.append(5);
    tcpSocket->write(buf);//发送指令

    tcpSocket->disconnectFromHost();//断开连接

    tcpSocket=NULL;
    emit game_close(is_red,steps.size());//发送信号
}

void serverGame::click(int id, int row, int col)
{
    if(!tcpSocket)
    {
        QMessageBox::warning(this,"警告","客户端未连接");
        return;//客户端为连接不能走棋
    }
    if(is_red != side)//不到自己不能走棋
        return;

    tool::click(id, row, col);//调用父函数

    /* 发送给对方 */
    QByteArray buf;
    buf.append(2);//2代表走棋指令
    buf.append(id);
    buf.append(row);
    buf.append(col);
    tcpSocket->write(buf);//发送指令
    true_over(s[4].dead,s[20].dead);//判断是否结束。
}

void serverGame::send_chat(QString str)
{
    //写入聊天框
    panel->writeChat(str,true);

    /*发送给对方*/
    QByteArray buf;
    buf.append(4);
    buf.append(str);
    tcpSocket->write(buf);
}

bool serverGame::is_over(int)
{
    return false;
}

bool serverGame::true_over(bool s4,bool s20)
{
    if(s4||s20)
    {
        loser();//调用认输函数
        return true;
    }
    return false;
}

void serverGame::backFromNetwork(QByteArray)
{
    //悔棋
    backOne();
    backOne();
    panel->writeWindow(QString("%1悔棋。").arg(is_red?"红方":"黑方"));
}
void serverGame::clickFromNetwork(QByteArray buf)
{
    int begin=selected;
    tool::click(buf[1], 9-buf[2], 8-buf[3]);//走棋，因为棋盘翻转，所以取对称
    int end=selected;
    /*判断对方是否走棋，走棋则打开我发悔棋，认输按钮*/
    if(begin!=-1&&(!sameColor(begin,end)))
    {
        panel->setbuttonEnble(true);
    }
}
void serverGame::initFromNetwork(QByteArray buf)
{
    //初始化棋盘
    bool bRedSide = buf.at(1)>0?true:false;
    init(bRedSide);
}
void serverGame::loserFromNetwork(QByteArray)
{
    loser();//调用认输函数
}
void serverGame::chatFromNetwork(QByteArray buf)
{
    buf.remove(0,1);//去掉第一个数字指令
    QString info=buf;
    panel->writeChat(info,false);//写入聊天框
}

void serverGame::slotDataArrive()
{
    //取出套接字内容
    QByteArray buf = tcpSocket->readAll();
    switch (buf.at(0)) {
    case 1:
        initFromNetwork(buf);
        break;
    case 2:
        clickFromNetwork(buf);
        break;
    case 3:
        backFromNetwork(buf);
        break;
    case 4:
        chatFromNetwork(buf);
        break;
    case 5:
        loserFromNetwork(buf);
        break;
    default:
        break;
    }
}

void serverGame::moveStone(int moveid, int row, int col)
{
    tool::moveStone(moveid,row,col);//调用父函数
    panel->setbuttonEnble(false);//关闭我方悔棋，认输按钮
    /*重置计数器*/
    panel->perTime_end();
    panel->perTime_begin();
    //改变行动方显示
    panel->change_side(is_red);
}
