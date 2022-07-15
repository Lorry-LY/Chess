#ifndef SINGELGAME_H
#define SINGELGAME_H
#include"tool.h"
#include"step.h"
#include"ctrlpanel.h"
class singelGame : public tool
{
public:
    singelGame(QWidget *,int level,bool side);
    ~singelGame();
    int level;//设置难度

    void back();//悔棋
    void click(int id, int row, int col);//选择
    void moveStone(int moveid, int row, int col) ;//走棋
    bool is_over(int killed);

    step* getBestMove();//获取最好的下一步
    void getAllPossibleMove(std::vector<step*>& steps);//获取所有可能的步骤
    int getMinScore(int level, int curMin);//计算并得到最低分
    int getMaxScore(int level, int curMax);//计算并得到最高分
    int score();//得分设置

    void fakeMove(step* step);//移动棋子
    void unfakeMove(step* step);//悔棋子

signals:

public slots:
    void computerMove();//人工智能移动棋子

private:
    Ctrlpanel* panel;//右控件
};

#endif // SINGELGAME_H
