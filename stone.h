#ifndef STONE_H
#define STONE_H
#include<QString>

class Stone
{
public:
    Stone();
    //定义枚举
    enum Type{JIANG,SHI,CHE,MA,XIANG,PAO,BING};
    //定义棋子属性
    int row;
    int col;
    Type type;
    bool dead;
    bool red;
    int id;
    //棋子初始化
    void init(int i);
    //翻转棋盘
    void reversal();
    //获取棋子名称
    QString name();
};

#endif // STONE_H
