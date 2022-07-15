#include "tool.h"
#include<QPainter>
#include<QMouseEvent>
#include<QMessageBox>
#include<QPushButton>
#include<QCoreApplication>
tool::tool(bool _side,QWidget *parent) : QWidget(parent)
{
    setWindowTitle("人机对战");
    resize(1100,900);
    init(_side);
}

tool::~tool()
{
    //释放步数容器内存
    for(int i=0;i<steps.size();i++)
    {
        step* temp=steps.back();
        steps.pop_back();
        delete temp;
    }
}

//初始化棋局
void tool::init(bool red_side)
{
    for(int i=0;i<32;i++)
    {
        s[i].init(i);//初始化棋子
    }
    if(red_side)//红旗的话翻转棋盘
    {
        for(int i=0;i<32;i++)
        {
            s[i].reversal();
        }
    }
    selected=-1;//初始化选棋
    is_red=true;//红旗先走
    side=red_side;//选编
    update();//重绘
}

//画画
void tool::paintEvent(QPaintEvent *)
{
    //定义画家
    QPainter p(this);

    p.save();
    p.drawPixmap(rect(),QPixmap(":/new/image/back.jpg"));
    p.restore();

    //画边框网格
    p.save();
    drawPlate(p);
    p.restore();
    //画九宫格
    p.save();
    drawPlace(p);
    p.restore();
    //画米字格
    p.save();
    drawInitPosition(p);
    p.restore();
    //画棋子
    p.save();
    drawStone(p);
    p.restore();
}
//画边框网格
void tool::drawPlate(QPainter &p)
{
    for(int i=0; i<10; ++i)
    {
        if(i==0 || i==9)
        {
            p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
        }
        else
        {
            p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
        }
        p.drawLine(center(i,0),center(i,8));
    }

    for(int i=0; i<9; ++i)
    {
        if(i==0 || i==8)
        {
            p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
            p.drawLine(center(0, i), center(9, i));
        }
        else
        {
            p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
            p.drawLine(center(0, i), center(4, i));
            p.drawLine(center(5, i), center(9, i));
        }
    }
}
//画九宫格
void tool::drawPlace(QPainter &p)
{
    p.setPen(QPen(Qt::black,1,Qt::SolidLine));
    p.drawLine(center(0, 3), center(2, 5));
    p.drawLine(center(2, 3), center(0, 5));
    p.drawLine(center(9, 3), center(7, 5));
    p.drawLine(center(7, 3), center(9, 5));
}
//画米字格
void tool::drawInitPosition(QPainter &p)
{
    drawInitPosition(p, 3, 0);
    drawInitPosition(p, 3, 2);
    drawInitPosition(p, 3, 4);
    drawInitPosition(p, 3, 6);
    drawInitPosition(p, 3, 8);
    drawInitPosition(p, 6, 0);
    drawInitPosition(p, 6, 2);
    drawInitPosition(p, 6, 4);
    drawInitPosition(p, 6, 6);
    drawInitPosition(p, 6, 8);
    drawInitPosition(p, 2, 1);
    drawInitPosition(p, 2, 7);
    drawInitPosition(p, 7, 1);
    drawInitPosition(p, 7, 7);
}
//画米字格
void tool::drawInitPosition(QPainter &p, int row, int col)
{
    if(col==0)
    {
        p.drawLine(center(row,col)+QPoint(10,-10),center(row,col)+QPoint(35,-10));
        p.drawLine(center(row,col)+QPoint(10,-10),center(row,col)+QPoint(10,-35));
        p.drawLine(center(row,col)+QPoint(10,10),center(row,col)+QPoint(10,35));
        p.drawLine(center(row,col)+QPoint(10,10),center(row,col)+QPoint(35,10));
    }
    else if(col==8)
    {

        p.drawLine(center(row,col)+QPoint(-10,-10),center(row,col)+QPoint(-10,-35));
        p.drawLine(center(row,col)+QPoint(-10,-10),center(row,col)+QPoint(-35,-10));
        p.drawLine(center(row,col)+QPoint(-10,10),center(row,col)+QPoint(-10,35));
        p.drawLine(center(row,col)+QPoint(-10,10),center(row,col)+QPoint(-35,10));
    }
    else
    {
        p.drawLine(center(row,col)+QPoint(10,-10),center(row,col)+QPoint(35,-10));
        p.drawLine(center(row,col)+QPoint(10,-10),center(row,col)+QPoint(10,-35));
        p.drawLine(center(row,col)+QPoint(10,10),center(row,col)+QPoint(10,35));
        p.drawLine(center(row,col)+QPoint(10,10),center(row,col)+QPoint(35,10));
        p.drawLine(center(row,col)+QPoint(-10,-10),center(row,col)+QPoint(-10,-35));
        p.drawLine(center(row,col)+QPoint(-10,-10),center(row,col)+QPoint(-35,-10));
        p.drawLine(center(row,col)+QPoint(-10,10),center(row,col)+QPoint(-10,35));
        p.drawLine(center(row,col)+QPoint(-10,10),center(row,col)+QPoint(-35,10));
    }
}
//画棋子
void tool::drawStone(QPainter &p)
{
    for(int i=0; i<32; i++)
    {
        drawStone(p, i);
    }
}
void tool::drawStone(QPainter &p, int id)
{
    if(isDead(id))
    {
        return;//不画死棋
    }
    QColor color;//定义颜色
    if(s[id].red)
    {
        color=Qt::red;//红棋为红色
    }
    else
    {
        color=Qt::black;//黑棋为黑色
    }
    p.setPen(QPen(QBrush(color), 2));
    if(id == selected) p.setBrush(Qt::gray);
    else p.setBrush(Qt::yellow);
    p.drawEllipse(cell(id));
    p.setFont(QFont("system", int(per_width*1.2), 700));
    p.drawText(cell(id), name(id), QTextOption(Qt::AlignCenter));
}


//获取中心坐标
QPoint tool::center(int row,int col)
{
    return QPoint(off+QPoint(col*per_width,row*per_width));
}
QPoint tool::center(int id)
{
    return QPoint(off+QPoint(s[id].col*per_width,s[id].row*per_width));
}

//获取左上角坐标
QPoint tool::topLeft(int row, int col)
{
    return QPoint(off+QPoint(col*per_width-per_width/2,row*per_width-per_width/2));
}
QPoint tool::topLeft(int id)
{
    return QPoint(off+QPoint(s[id].col*per_width-per_width/2,s[id].row*per_width-per_width/2));
}

//获取棋子边框
QRect tool::cell(int row, int col)
{
    return QRect(topLeft(row,col),QSize(per_width,per_width));
}
QRect tool::cell(int id)
{
    return QRect(topLeft(id),QSize(per_width,per_width));
}

//判断有无 点到棋盘内
bool tool::getClickRowCol(QPoint pt, int &row, int &col)//并保存行列值
{
    for(row=0;row<=9;row++)
    {
        for(col=0;col<=8;col++)
        {
            QPoint distance=center(row,col)-pt;
            if(distance.x()*distance.x()+distance.y()*distance.y()<=per_width*per_width/4)
            {
                return true;
            }
        }
    }
    return false;
}

//获取文件名字
QString tool::name(int id)
{
    return s[id].name();
}

//判断是不是红旗
bool tool::red(int id)
{
    return s[id].red;
}

//判断两个棋子是不是相同颜色
bool tool::sameColor(int id1, int id2)
{
    return s[id1].red==s[id2].red;
}

//获取某行某列的棋子
int tool::getStoneId(int row, int col)
{
    for(int i=0;i<32;i++)
    {
        if(s[i].dead==false&&s[i].row==row&&s[i].col==col)
        {
            return i;   //该出有棋子返回下标
        }
    }
    return -1;  //该出无棋子返回-1
}

//杀死棋子
void tool::killStone(int id)
{
    s[id].dead=true;
}

//复活棋子
void tool::reliveStone(int id)
{
    s[id].dead=false;
}

bool tool::isDead(int id)
{
    return s[id].dead;
}

//鼠标放开事件
void tool::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)    //不是左键不执行
    {
        click(ev->pos());   //获取左键放开坐标
        return;
    }    
    else if(ev->button()==Qt::RightButton)
    {
        this->selected=-1;
        update();
        return;
    }
    else
    {
        return;
    }
}

//选择棋子
void tool::click(QPoint pt)
{
    if(over)
    {
        return;//游戏结束
    }
    int row,col;
    if(getClickRowCol(pt,row,col))  //判断是不是点到了棋盘内
    {
        int id=getStoneId(row,col);     //获取该为位置棋子id
        click(id,row,col);          //获取棋子
    }
}
void tool::click(int id, int row, int col)
{
    if(this->selected==-1)  //判断有没有棋子已被选中
    {
        trySelectStone(id);         //尝试选择棋子
    }
    else
    {
        tryMoveStone(id,row,col);       //尝试移动棋子
    }
}

//尝试选棋
void tool::trySelectStone(int id)
{
    if(id==-1)
    {
        return; //该处没有棋子
    }
    if(!canSelect(id))
    {
        return; //该棋子不可选择;
    }
    this->selected=id;
    update();   //重绘
}

//能不能选这个棋子
bool tool::canSelect(int id)
{
    return is_red==s[id].red;
}

//尝试走棋
void tool::tryMoveStone(int killid, int row, int col)
{
    if(killid!=-1&&sameColor(killid,selected))  //选择相同颜色的棋子时
    {
        trySelectStone(killid);     //重新选择棋子
        this->selected=killid;
        return;
    }
    if(canMove(selected,killid,row,col))    //规则允许移动棋子
    {
        moveStone(selected,killid,row,col);     //移动棋子
        this->selected=-1;  //清除选择
        update();   //重绘
    }
}

//走棋
void tool::moveStone(int moveid, int killid, int row, int col)
{
    saveStep(moveid,killid,row,col,steps);  //保存步数
    killStone(killid);  //杀死棋子
    moveStone(moveid,row,col);
    over=is_over(killid);

}
void tool::moveStone(int moveid, int row, int col)
{
    s[moveid].row=row;
    s[moveid].col=col;
    is_red=!is_red; //换边
}

//保存该步
void tool::saveStep(int moveid, int killid, int row, int col, std::vector<step *>& steps)
{
    step *temp=new step;
    temp->moveid=moveid;
    temp->killid=killid;
    temp->row_begin=s[moveid].row;
    temp->col_begin=s[moveid].col;
    temp->row_end=row;
    temp->col_end=col;
    steps.push_back(temp);
}

//判断结束不
bool tool::is_over(int killed)
{
    if(killed==4||killed==20)
    {
        emit game_close(is_red,steps.size());
        return true;
    }
    return false;
}

//悔棋
void tool::back()
{
    backOne();
}
void tool::backOne()
{
    if(steps.size()==0)
    {
        return;//没有悔棋可走
    }
    step *temp=steps.back();    //接收最后一步
    steps.pop_back();   //弹出最后一步
    back(temp);     //悔棋

    update();       //重绘
    delete temp;    //释放内存空间
}
void tool::back(step *step)
{
    reliveStone(step->killid);  //复活棋子
    moveStone(step->moveid,step->row_begin,step->col_begin);    //移动棋子
}

//规则
bool tool::canMove(int moveid, int killid, int row, int col)
{
    if(killid!=-1&&sameColor(moveid,killid))
    {
        return false;   //不能杀死自家棋子
    }
    switch (s[moveid].type)
    {
    case Stone::CHE:
        return canMoveChe(moveid,killid,row,col);
    case Stone::MA:
        return canMoveMa(moveid,killid,row,col);
    case Stone::XIANG:
        return canMoveXiang(moveid,killid,row,col);
    case Stone::SHI:
        return canMoveShi(moveid,killid,row,col);
    case Stone::JIANG:
        return canMoveJiang(moveid,killid,row,col);
    case Stone::BING:
        return canMoveBing(moveid,killid,row,col);
    case Stone::PAO:
        return canMovePao(moveid,killid,row,col);
    }
    return false;
}
//车
bool tool::canMoveChe(int moveid, int , int row, int col)
{
    if(s[moveid].row!=row&&s[moveid].col!=col)
    {
        return false; //车不能斜着走
    }
    if(getStoneCountAtLine(s[moveid].row,s[moveid].col,row,col))
    {
        return false; //中间不能跳棋
    }
    return true;
}
//马
bool tool::canMoveMa(int moveid, int, int row, int col)
{
    int distance=relation(s[moveid].row,s[moveid].col,row,col);
    if(distance!=12&&distance!=21)
    {
        return false;//走日字格
    }
    /*卡马脚*/
    if(qAbs(s[moveid].row-row)==1)//横着走
    {
        if(s[moveid].col<col)//右走
        {
            for(int i=0;i<32;i++)
            {
                if(s[i].dead==false&&s[i].row==s[moveid].row&&s[i].col==col-1)
                {
                    return false;
                }
            }
        }
        else//右走
        {
            for(int i=0;i<32;i++)
            {
                if(s[i].dead==false&&s[i].row==s[moveid].row&&s[i].col==col+1)
                {
                    return false;
                }
            }
        }
    }
    else    //竖着走
    {
        if(s[moveid].row<row)//下走
        {
            for(int i=0;i<32;i++)
            {
                if(s[i].dead==false&&s[i].col==s[moveid].col&&s[i].row==row-1)
                {
                    return false;
                }
            }
        }
        else//上走
        {
            for(int i=0;i<32;i++)
            {
                if(s[i].dead==false&&s[i].col==s[moveid].col&&s[i].row==row+1)
                {
                    return false;
                }
            }
        }
    }
    return true;
}
//象
bool tool::canMoveXiang(int moveid, int , int row, int col)
{
    int distance=relation(s[moveid].row,s[moveid].col,row,col);
    if(distance!=22)//走田字格
    {
        return false;
    }
    //象不能过河
    if(isBottomSide(moveid))
    {
        if(row<4)
        {
            return false;
        }
    }
    else
    {
        if(row>5)
        {
            return false;
        }
    }
    //田中无棋子
    for(int i=0;i<32;i++)
    {
        if(s[i].dead==false&&s[i].row==(s[moveid].row+row)/2&&s[i].col==(s[moveid].col+col)/2)
        {
            return false;
        }
    }
    return true;
}
//士
bool tool::canMoveShi(int moveid, int , int row, int col)
{
    int distance=relation(s[moveid].row,s[moveid].col,row,col);
    if(distance!=11)//走斜线
    {
        return false;
    }
    //士不能出米字格
    if(isBottomSide(moveid))
    {
        if(row<7||col<3||col>5)
        {
            return false;
        }
    }
    else
    {

        if(row>2||col<3||col>5)
        {
            return false;
        }
    }
    return true;
}
//将
bool tool::canMoveJiang(int moveid, int killid, int row, int col)
{

    //将帅碰面直接吃
    if(killid!=-1&&s[killid].type==Stone::JIANG&&s[moveid].col==col)
    {
        if(!getStoneCountAtLine(s[moveid].row,s[moveid].col,row,col))
        {
            return true;
        }
    }
    int distance=relation(s[moveid].row,s[moveid].col,row,col);
    if(distance!=1&&distance!=10)
    {
        return false;//只能走一格;
    }
    //走单格子
    if(distance!=1&&distance!=10)
    {
        return false;
    }
    //不能出米字格
    if(isBottomSide(moveid))
    {
        if(row<7||col<3||col>5)
        {
            return false;
        }
    }
    else
    {

        if(row>2||col<3||col>5)
        {
            return false;
        }
    }
    return true;
}
//兵
bool tool::canMoveBing(int moveid, int , int row, int col)
{
    int distance=relation(s[moveid].row,s[moveid].col,row,col);
    if(distance!=1&&distance!=10)
    {
        return false;//只能走一格
    }
    if(isBottomSide(moveid))
    {
        if(row>s[moveid].row)//兵不能后退
        {
            return false;
        }
        if(row>4&&s[moveid].col!=col)//过河之前不能横着走
        {
            return false;
        }
    }
    else
    {
        if(row<s[moveid].row)//兵不能后退
        {
            return false;
        }
        if(row<5&&s[moveid].col!=col)//过河之前不能横着走
        {
            return false;
        }
    }
    return true;
}
//炮
bool tool::canMovePao(int moveid, int killid, int row, int col)
{
    if(killid==-1)//炮走路
    {
        return canMoveChe(moveid,killid,row,col);
    }
    else//炮打人
    {
        if(s[moveid].row!=row&&s[moveid].col!=col)
        {
            return false; //炮不能斜着打
        }
        int stone=getStoneCountAtLine(s[moveid].row,s[moveid].col,row,col);
        if(stone!=1)
        {
            return false;   //只能跳一个棋子打;
        }
    }
    return true;
}

/*规则辅助函数*/
//获取某行或者某列两点间棋子数目
int tool::getStoneCountAtLine(int row1, int col1, int row2, int col2)
{
    int number=0;
    if(row1==row2)
    {
        for(int i=min(col1,col2)+1;i<max(col1,col2);i++)
        {
            for(int j=0;j<32;j++)
            {
                if(s[j].dead==false&&s[j].row==row1&&s[j].col==i)
                {
                    number++;
                }
            }
        }
    }
    if(col1==col2)
    {
        for(int i=min(row1,row2)+1;i<max(row1,row2);i++)
        {
            for(int j=0;j<32;j++)
            {
                if(s[j].dead==false&&s[j].col==col1&&s[j].row==i)
                {
                    number++;
                }
            }
        }
    }
    return number;
}
//得到两点间距离
int tool::relation(int row1, int col1, int row, int col)
{
    return qAbs(row1-row)*10+qAbs(col1-col);
}
//判断那边棋子在下部
bool tool::isBottomSide(int id)
{
    return side==s[id].red;
}
//最小值
int tool::min(int a, int b)
{
    return a<b?a:b;
}
//最大值
int tool::max(int a,int b)
{
    return a>b?a:b;
}

