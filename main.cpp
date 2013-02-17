#include <QApplication>
#include <QtGui>
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(codec);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
