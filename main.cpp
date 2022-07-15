#include "mywidget.h"

#include <QApplication>
#include<QMessageBox>
#include<QIcon>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyWidget w;
    w.setWindowIcon(QIcon(":/new/image/logo.ico")); //设置图标
    return a.exec();
}
