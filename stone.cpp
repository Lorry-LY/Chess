#include "stone.h"

Stone::Stone()
{
//    for(int i=0;i<32;i++)
//    {
//        init(i);
//    }
}

void Stone::init(int id)
{
    //定义一半棋盘结构体
    struct {
        int row;
        int col;
        Stone::Type type;
    } pos[16] = {
    {0, 0, Stone::CHE},
    {0, 1, Stone::MA},
    {0, 2, Stone::XIANG},
    {0, 3, Stone::SHI},
    {0, 4, Stone::JIANG},
    {0, 5, Stone::SHI},
    {0, 6, Stone::XIANG},
    {0, 7, Stone::MA},
    {0, 8, Stone::CHE},

    {2, 1, Stone::PAO},
    {2, 7, Stone::PAO},
    {3, 0, Stone::BING},
    {3, 2, Stone::BING},
    {3, 4, Stone::BING},
    {3, 6, Stone::BING},
    {3, 8, Stone::BING},
};
    //定义上半部棋子
    if(id < 16)
    {
        this->col = pos[id].col;
        this->row = pos[id].row;
        this->type = pos[id].type;
    }
    //定义下半部棋子
    else
    {
        this->col = 8-pos[id-16].col;
        this->row = 9-pos[id-16].row;
        this->type = pos[id-16].type;
    }
    //初始化棋子未死
    this->dead = false;
    //设定棋子颜色
    this->red = id<16;
}

//获取棋子名字
QString Stone::name()
{
    switch(this->type)
    {
    case CHE:
        return "车";
    case MA:
        return "马";
    case PAO:
        return "炮";
    case BING:
        return "兵";
    case JIANG:
        return "将";
    case SHI:
        return "士";
    case XIANG:
        return "相";
    }
    return "错误";
}

//翻转棋盘
void Stone::reversal()
{
    this->col = 8-this->col;
    this->row = 9-this->row;
}

