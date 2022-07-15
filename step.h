#ifndef STEP_H
#define STEP_H

#include <QObject>

class step : public QObject
{
    Q_OBJECT
public:
    explicit step(QObject *parent = nullptr);


    int moveid;//移动棋子
    int killid; //被杀棋子
    int row_begin;//移动前所在行
    int col_begin;//移动前所在列
    int row_end;//移动后所在行
    int col_end;//移动后所在列

signals:

public slots:
};

#endif // STEP_H
