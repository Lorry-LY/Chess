#ifndef TOOL_H
#define TOOL_H

#include <QWidget>
#include<vector>
#include"stone.h"
#include"step.h"
#include"dialog_login.h"
class tool : public QWidget
{
    Q_OBJECT

private:
    Dialog_login dia;
public:
    explicit tool(bool side,QWidget *parent = nullptr);
    ~tool();

    /*初始化游戏数据*/
    Stone s[32];//32颗棋子
    bool side;//选编
    std::vector<step*>steps;//记录每步容器
    void init(bool red_side);//初始化棋局
    int per_width=80;//初始化格子边框
    QPoint off=QPoint(80,80);//初始化棋盘偏移走上角的位置
    bool over=false;//游戏结束标志

    /*游戏过程用到变量*/
    int selected;//被选中棋子
    bool is_red;//是不是红方走棋

    /* 绘图 */
    void paintEvent(QPaintEvent *);//重置绘图事件
    void drawPlate(QPainter& p); //画棋盘横竖线
    void drawPlace(QPainter& p); //画九宫格
    void drawInitPosition(QPainter& p);//画米字格
    void drawInitPosition(QPainter& p, int row, int col);//画单个米字格
    void drawStone(QPainter& p); //画棋子
    void drawStone(QPainter& p, int id); //画单个棋子

    /*坐标转换函数*/
    QPoint center(int row, int col);//取得某行某列坐标
    QPoint center(int id);//取得棋子坐标
    QPoint topLeft(int row, int col);//取得某行某列左上角坐标
    QPoint topLeft(int id);//取得棋子坐标
    QRect cell(int row, int col);//取得某行某列边框
    QRect cell(int id);//取得棋子边框

    bool getClickRowCol(QPoint pt, int& row, int& col);//判断是否点击到棋盘内

    /*辅助函数*/
    QString name(int id);//获取棋子名字
    bool red(int id);//判断棋子是不是红旗
    bool sameColor(int id1, int id2);//判断两个棋子是不是相同颜色
    int getStoneId(int row, int col);//获取某行某列的棋子id
    void killStone(int id);//杀死棋子
    void reliveStone(int id);//复活棋子
    bool isDead(int id);//判断棋子死没死
    virtual bool is_over(int killed);//判断是否结束棋局

    /*移动棋子*/
    virtual void moveStone(int moveid, int row, int col);//只移动棋子
    void mouseReleaseEvent(QMouseEvent *);//重置鼠标释放事件
    void click(QPoint pt);//选择棋盘
    virtual void click(int id, int row, int col);//选择棋子
    void trySelectStone(int id);//选中棋子
    void cancelSelect();//取消选择
    void tryMoveStone(int killid, int row, int col);//移动棋子
    void moveStone(int moveid, int killid, int row, int col);//移动棋子并杀死目标
    void saveStep(int moveid, int killid, int row, int col, std::vector<step*>& steps);//保存移动步数信息
    void backOne();//悔棋一步
    void back(step* step);//悔棋某步
    virtual void back();//悔棋槽函数

    /*规则制定*/
    bool canMove(int moveid, int killid, int row, int col); //移动总规则
    bool canMoveChe(int moveid, int killid, int row, int col);//车的移动规则
    bool canMoveMa(int moveid, int killid, int row, int col);//马的移动规则
    bool canMovePao(int moveid, int killid, int row, int col);//炮的移动规则
    bool canMoveBing(int moveid, int killid, int row, int col);//兵的移动规则
    bool canMoveJiang(int moveid, int killid, int row, int col);//将的移动规则
    bool canMoveShi(int moveid, int killid, int row, int col);//士的移动规则
    bool canMoveXiang(int moveid, int killid, int row, int col);//象的移动规则

    bool canSelect(int id);//能不能选择该棋子

    /*规则辅助函数*/
    int getStoneCountAtLine(int row1, int col1, int row2, int col2);//获得某行或者某列的棋子数目
    int relation(int row1, int col1, int row, int col);//用特殊方法计算"两点就距离"
    bool isBottomSide(int id);//判断那方棋在下面
    int min(int a,int b);//返回最小值
    int max(int a,int b);//返回最大值

signals:
    void game_close(bool is_red,int number);//关闭


};

#endif // TOOL_H
