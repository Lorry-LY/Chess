#include "singelgame.h"
#include <QTimer>
#include <QDebug>
singelGame::singelGame(QWidget *parent,int _level,bool _side) : tool(_side,parent)
{
    level = 2+2*_level;  //设置难度
    side=_side;//设置

    panel=new Ctrlpanel(2,this);//右控件
    /*建立悔棋，认输，超时信号与槽*/
    connect(panel,&Ctrlpanel::_back,this,&singelGame::back);
    connect(panel,&Ctrlpanel::_loser,
            [=]()
    {
        emit game_close(is_red,steps.size());
    }
    );
    connect(panel,&Ctrlpanel::_overtime,
            [=](){
        panel->writeWindow(QString("%1超时,交换走棋。").arg(is_red?"红方":"黑方"));
        moveStone(0,s[0].row,s[0].col);
    });

    panel->Alltime_begin();//打开总计时期
    panel->perTime_begin();//打开每步计时器
}

singelGame::~singelGame()
{

}

//悔棋
void singelGame::back()
{
    if(is_red)//悔棋悔两步
    {
        backOne();
        backOne();
    }
}

//点击
void singelGame::click(int id, int row, int col)
{
    if(over)
    {
        return;//游戏结束
    }
    if(is_red)//红方人工走
    {
        tool::click(id, row, col);
        if(!is_red)//走棋换边
        {
            QTimer::singleShot(1000, this, &singelGame::computerMove);//延时器，1秒之后走棋
        }
    }
}

//走棋
void singelGame::moveStone(int moveid, int row, int col)
{
    tool::moveStone(moveid,row,col);
    panel->setbuttonEnble(side);
    panel->perTime_end();
    panel->perTime_begin();
    panel->change_side(is_red);
}

//判断结束
bool singelGame::is_over(int killed)
{
    if(killed==4||killed==20)
    {
        //结束计时器
        panel->Alltime_end();
        panel->perTime_end();
        emit game_close(is_red,steps.size());//发送结束信号
        return true;
    }
    return false;
}

//人机走棋
void singelGame::computerMove()
{
    step* step = getBestMove(); //得到最好的步数
    tool::moveStone(step->moveid, step->killid, step->row_end, step->col_end);    //走棋
    delete step;//释放空间
    update();//重绘
}

step* singelGame::getBestMove()
{
    step* ret = new step;
    std::vector<step*> allSteps; //步数容器
    getAllPossibleMove(allSteps); //获取所有的步数
    int maxInAllMinScore = -300000;

    while(allSteps.size())
    {
        step* step = allSteps.back();  //弹出最后一步
        allSteps.pop_back();//删除最后一步
        fakeMove(step);//走棋
        int minScore = getMinScore(this->level-1, maxInAllMinScore);//计算最小分
        unfakeMove(step);//还原棋

        if(minScore > maxInAllMinScore)
        {
            if(ret) delete ret;//清空ret
            ret = step;
            maxInAllMinScore = minScore;
        }
        else
        {
            delete step;
        }
    }
    return ret;
}

//计算分数函数
int singelGame::score()
{
    //  enum TYPE{CHE, MA, PAO, BING, JIANG, SHI, XIANG};
    static int s_score[] = {1000, 499, 501, 200, 100000, 100, 100};
    int scoreBlack = 0;
    int scoreRed = 0;
    for(int i=0; i<16; ++i)
    {
        if(s[i].dead) continue;
        scoreRed += s_score[s[i].type];
    }
    for(int i=16; i<32; ++i)
    {
        if(s[i].dead) continue;
        scoreBlack += s_score[s[i].type];
    }
    return scoreBlack - scoreRed;
}

int singelGame::getMinScore(int level, int curMin)
{
    if(level == 0)//递归终止条件
    {
        return score();
    }
    std::vector<step*> allSteps;
    getAllPossibleMove(allSteps);//取得所有可能性
    int minInAllMaxScore = 300000;

    while(allSteps.size())
    {
        //取出最后一步
        step* step = allSteps.back();
        allSteps.pop_back();

        fakeMove(step);//走棋
        int maxScore = getMaxScore(level-1, minInAllMaxScore);//计算最大值
        unfakeMove(step);//还原
        delete step;//释放空间

        if(maxScore <= curMin)//如果最大分数小于当前分数，就释放容器，返回最大分数
        {
            for(int i=0;i<allSteps.size();i++)
            {
                delete allSteps[i];
            }
            return maxScore;
        }

        if(maxScore < minInAllMaxScore)//如果最大值小于所有最大值里最小的那个，求替换
        {
            minInAllMaxScore = maxScore;
        }


    }
    return minInAllMaxScore;//返回最大值里最小的值
}
int singelGame::getMaxScore(int level, int curMax)
{
    if(level == 0)//递归结束条件
        return score();

    std::vector<step*> allSteps;
    getAllPossibleMove(allSteps);//取得所有步数
    int maxInAllMinScore = -300000;

    while(allSteps.size())
    {
        step* step = allSteps.back();
        allSteps.pop_back();//取出最后一步

        fakeMove(step);//走棋
        int minScore = getMinScore(level-1, maxInAllMinScore);//计算最小值
        unfakeMove(step);//还原棋
        delete step;//释放空间

        if(minScore >= curMax)//如果最小值大于当前分数，退出返回最小值
        {
            for(int i=0;i<allSteps.size();i++)
            {
                delete allSteps[i];
            }
            return minScore;
        }
        if(minScore > maxInAllMinScore)//如果最小值大于最小值分数里的最大值，成为最大值
        {
            maxInAllMinScore = minScore;
        }


    }
    return maxInAllMinScore;//返回最小值里的最大值
}

//走棋
void singelGame::fakeMove(step *step)
{
    killStone(step->killid);
    moveStone(step->moveid, step->row_end, step->col_end);
}
//还原棋
void singelGame::unfakeMove(step *step)
{
    reliveStone(step->killid);
    moveStone(step->moveid, step->row_begin, step->col_begin);
}

//取得所有可能步数
void singelGame::getAllPossibleMove(std::vector<step *> &allSteps)
{
    int min, max;   //设置棋子范围
    if(this->is_red)
    {
        min = 0;
        max = 16;
    }
    else
    {
        min = 16;
        max = 32;
    }

    for(int i=min;i<max; i++)
    {
        if(this->s[i].dead) //死了就不走棋
        {
            continue;
        }
        for(int row = 0; row<=9; ++row)
        {
            for(int col=0; col<=8; ++col)
            {
                int killid = this->getStoneId(row, col);
                if(killid!=-1&&sameColor(i, killid)) //相同棋子不杀
                {
                    continue;
                }
                if(canMove(i, killid, row, col))//判断能走
                {
                    saveStep(i, killid, row, col, allSteps);//保存步数
                }
            }
        }
    }
}
