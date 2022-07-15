#include "netgame.h"

netGame::netGame(QWidget* parent,bool _server,QString ip,QString port) : tool(_server,parent)
{
    server = nullptr;
    socket = nullptr;
    is_server = _server;

    if(is_server)
    {

        server = new QTcpServer(this);
        server->listen(QHostAddress::Any,1314);
        connect(server,&QTcpServer::newConnection,this,&netGame::slotNewConnection);
        panel=new Ctrlpanel(3,this);


    }
    else
    {
        socket = new QTcpSocket(this);
        socket->connectToHost(QHostAddress(ip), port.toShort());
        connect(socket, &QTcpSocket::readyRead, this, &netGame::slotDataArrive);
        panel=new Ctrlpanel(4,this);
        connect(panel,&Ctrlpanel::_back,this,&netGame::back);
        connect(panel,&Ctrlpanel::_loser,
                [=]()
        {
            emit game_close(is_red,steps.size());

            /*把服务器认输的消息告诉对方*/
            QByteArray buf;
            buf.append(5);
            socket->write(buf);
        });
    }


}

void netGame::slotNewConnection()
{
//    if(socket)
//    {
//        //提醒连接失败
//        QMessageBox::warning(this,"连接失败","已有客户端连入服务器。");
//        return;//客户端已连接
//    }


    //取出套接字
    socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &netGame::slotDataArrive);
    connect(panel,&Ctrlpanel::_back,this,&netGame::back);
    connect(panel,&Ctrlpanel::_loser,
            [=]()
    {
        emit game_close(is_red,steps.size());

        /*把认输的信息告诉对方。*/
        QByteArray buf;
        buf.append(5);
        socket->write(buf);
    });

    /* 产生随机数来决定谁走红色 */
    bool bRedSide = qrand()%2>0;
    init(bRedSide);

    /* 发送给对方 */
    QByteArray buf;
    buf.append(1);//1:表示选编
    buf.append(bRedSide?0:1);//把对边放入数组
    socket->write(buf);
}

void netGame::back()
{
    if(is_red != side)
    {
        return;//不到自己出牌，不能悔棋
    }
    backOne();
    backOne();

    QByteArray buf;
    buf.append(3);//3代表悔棋指令
    socket->write(buf);
}

void netGame::click(int id, int row, int col)
{
    if(!socket)
    {
        QMessageBox::warning(this,"警告","客户端未连接");
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

    socket->write(buf);
    true_over(s[4].dead,s[20].dead);//判断是否结束。
}

bool netGame::is_over(int)
{
    return false;
}

bool netGame::true_over(bool s4,bool s20)
{
    if(s4||s20)
    {
        emit game_close(is_red,steps.size());
        return true;
    }
    return false;
}

void netGame::backFromNetwork(QByteArray)
{
    backOne();
    backOne();
}
void netGame::clickFromNetwork(QByteArray buf)
{
    int begin=selected;
    tool::click(buf[1], 9-buf[2], 8-buf[3]);//走棋，因为棋盘翻转，所以取对称
    int end=selected;
    if(begin!=-1&&(!sameColor(begin,end)))
    {
        panel->setbuttonEnble(true);
    }
}
void netGame::initFromNetwork(QByteArray buf)
{
    bool bRedSide = buf.at(1)>0?true:false;
    init(bRedSide);
}
void netGame::loserFromNetwork(QByteArray)
{
    emit game_close(is_red,steps.size());
}

void netGame::slotDataArrive()
{
    //提示连接成功
    QByteArray buf = socket->readAll();
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
    case 5:
        loserFromNetwork(buf);
        break;
    default:
        break;
    }
}

void netGame::moveStone(int moveid, int row, int col)
{
    tool::moveStone(moveid,row,col);
    panel->setbuttonEnble(false);
}

netGame::~netGame()
{

}

