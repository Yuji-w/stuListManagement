#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pageShow.h"
#include "connectDb.h"
#include <QMainWindow>
#include <QList>
#include <QStringList>
#include <QButtonGroup>
#include <QTableWidgetItem>

//QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}

class Person
{
public:
    Person(){
    };
    enum ChooseSex
    {
        man,
        woman,
    };
    QString m_s_name;   //姓名
    int m_i_age;        //年龄
    ChooseSex m_sex;
};

class Student : public Person
{
public:
    Student()
    {
        number = 0;
    };

    void setNum(int m){number = m;}
    int getNum(){return number;}

    void setPer(QString per){perfess = per;}
    QString getPer(){return perfess;}

private:
    QString perfess;   //专业
    int number;        //学号
};

class Energer : public Person
{
public:
    Energer()
    {
        m_i_workAge = 0;
    };

    void setWorkAge(int w){m_i_workAge = w;}
    int getWorkAge(){return m_i_workAge;}

private:
    int m_i_workAge;    //工龄
};

class ConnectDb;
class PageShow;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    int getRowVaule();
    void setFormsShow(int currPage,int t_row);
    void changeComboBox(int index);
    void chooseRow();
    void setTabBackGround();
    void setRightClickMenu();

    bool isChangePage(int listLenth,int rowValue);
    bool informationMessage();

private slots:
    void slotAddData();
    void slotOpenFile();
    void slotSaveFile();
    void slotDelRow();
    void slotStuFileList(QTableWidgetItem *item);
    void slotEngFileList(QTableWidgetItem *item);
    void slotAddMore();
    void slotChangeItem();
    void slotChangeTab(int index);
    void slotChangeCombox(int index);  
    void slotContextMenu(QPoint pos);
    int  slotChangeRow();
    Person::ChooseSex slotEngSex();
    Person::ChooseSex slotSex();

    void slotNextPage();
    void slotFirstPage();
    void slotPrePage();
    void slotLastPage();
    void slotConnectSql();

private:
    Ui::MainWindow *ui;
    QList <Student> S_list;
    QList <Energer> E_list;

    int m_i_stuPage;    //学生类当前页
    int m_i_engPage;    //程序员类当前页

    QAction *m_pSaveAction;     //右键保存
    QAction *m_pDelAction;      //右键删除

    QMenu *m_pButtonMenu;       //右键菜单

    QButtonGroup *m_pStuSexGroup;    //学生类性别
    QButtonGroup *m_pEngSexGroup;    //程序员类性别

    PageShow* m_pPageShowEng;       //eng类翻页
    PageShow* m_pPageShowStu;       //stu类翻页

    ConnectDb* m_pConnectDb;        //sql操作
};
#endif // MAINWINDOW_H
