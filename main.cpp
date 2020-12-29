#include "mainwindow.h"
#include <QApplication>
#include <QCompleter>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFormLayout>
#include <QTextBrowser>
#include <QLayout>
#include <QTabWidget>
#include <QFile>
#include <QTextCodec>

#include <QSettings>  //模块化读取文件

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));



    MainWindow w;
    QFile qssFile(":/style.qss");
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
        QString qss = QLatin1String(qssFile.readAll());
        qApp->setStyleSheet(qss);
        qssFile.close();
    }



    w.show();

    return a.exec();
}
