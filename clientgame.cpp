#include "clientgame.h"
#include<QMessageBox>
clientGame::clientGame(bool _side,QWidget *parent,QString ip,QString port):tool(_side,parent)
{
    tcpSocket=NULL;
    panel=new Ctrlpanel(4,this);//创建右控件
    /*组织信号与槽*/
    connect(panel,&Ctrlpanel::_back,this,&clientGame::back);
    connect(panel,&Ctrlpanel::_loser,this,&clientGame::loser);
    connect(panel,&Ctrlpanel::_chat,this,&clientGame::send_chat);
    connect(panel,&Ctrlpanel::_overtime,
            [=](){
        panel->writeWindow(QString("%1超时,交换走棋。").arg(is_red?"红方":"黑方"));
        moveStone(0,s[0].row,s[0].col);
    });
    //打开计时器
    panel->Alltime_begin();
    panel->perTime_begin();

    tcpSocket = new QTcpSocket(this);//创建客户端
    tcpSocket->connectToHost(QHostAddress(ip), port.toShort());//连接服务器
    connect(tcpSocket, &QTcpSocket::readyRead, this, &clientGame::slotDataArrive);//建立监听信号与槽
    connect(tcpSocket,&QTcpSocket::connected,
            [=]()
    {
        panel->writeWindow("成功与服务器连接成功。");//写入系统框
        panel->setenble();//打开聊天按钮
    }
            );

}

void clientGame::back()
{
    if(is_red != side)
    {
        return;//不到自己出牌，不能悔棋
    }
    backOne();
    backOne();

    QByteArray buf;
    buf.append(3);//3代表悔棋指令
    tcpSocket->write(buf);
    panel->writeWindow(QString("%1悔棋。").arg(is_red?"红方":"黑方"));
}

void clientGame::loser()
{
    panel->Alltime_end();
    panel->perTime_end();
    /*发送认输消息*/
    QByteArray buf;
    buf.append(5);
    tcpSocket->write(buf);

    tcpSocket->disconnectFromHost();//断开连接

    tcpSocket=NULL;

    emit game_close(is_red,steps.size());
}

void clientGame::click(int id, int row, int col)
{
    if(!tcpSocket)
    {
        QMessageBox::warning(this,"警告","服务器未连接");
        return;//客户端为连接不能走棋
    }
    if(is_red != side)//不到自己不能走棋
        return;

    tool::click(id, row, col);

    /* 发送给对方 */
    QByteArray buf;
    buf.append(2);//2代表走棋指令
    buf.append(id);
    buf.append(row);
    buf.append(col);
    tcpSocket->write(buf);
    true_over(s[4].dead,s[20].dead);//判断是否结束。
}

void clientGame::send_chat(QString str)
{
    panel->writeChat(str,true);

    /*发送给对方*/
    QByteArray buf;
    buf.append(4);
    buf.append(str);
    tcpSocket->write(buf);
}

bool clientGame::is_over(int)
{
    return false;
}

bool clientGame::true_over(bool s4,bool s20)
{
    if(s4||s20)
    {
        loser();//调用函数
        return true;
    }
    return false;
}

void clientGame::backFromNetwork(QByteArray)
{
    //悔棋两步
    backOne();
    backOne();
    panel->writeWindow(QString("%1悔棋。").arg(is_red?"红方":"黑方"));
}
void clientGame::clickFromNetwork(QByteArray buf)
{
    int begin=selected;
    tool::click(buf[1], 9-buf[2], 8-buf[3]);//走棋，因为棋盘翻转，所以取对称
    int end=selected;
    if(begin!=-1&&(!sameColor(begin,end)))
    {
        panel->setbuttonEnble(true);//打开按钮控件
    }
}
void clientGame::initFromNetwork(QByteArray buf)
{
    //初始化
    bool bRedSide = buf.at(1)>0?true:false;
    init(bRedSide);
}
void clientGame::loserFromNetwork(QByteArray)
{
    loser();//调用函数
}
void clientGame::chatFromNetwork(QByteArray buf)
{
    buf.remove(0,1);//去除指令数字
    QString info=buf;
    panel->writeChat(info,false);//写入聊天框
}

void clientGame::slotDataArrive()
{
    //取出套接字
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

void clientGame::moveStone(int moveid, int row, int col)
{
    tool::moveStone(moveid,row,col);//调用父函数
    panel->setbuttonEnble(false);//关闭按钮控件
    /*重置每步计时器*/
    panel->perTime_end();
    panel->perTime_begin();
    //换边
    panel->change_side(is_red);
}
