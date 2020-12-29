#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

    QString m_s_name;
    int m_i_age;
    QString m_s_sex;
};

class Student : public Person
{
public:
    Student()
    {
    };

    void setNum(int m){number = m;}
    int getNum(){return number;}

    void setPer(QString per){perfess = per;}
    QString getPer(){return perfess;}

private:
    QString perfess;
    int number;
};

class Energer : public Person
{
public:
    Energer()
    {
    };

    void setWorkAge(int w){m_i_workAge = w;}
    int getWorkAge(){return m_i_workAge;}

private:
    int m_i_workAge;
};

enum LeiXin
{
    Stu,
    Ene
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    int getRowVaule();

    void changeComboBox(int index);
    void setFormsShow(int currPage,int t_row);
    void chooseRow();
    void setTabBackGround();
    void setRightClickMenu();

    bool isChangePage(int listLenth,int rowValue);
    bool informationMessage();

    Student sqlDataQuery();
    bool isInsertData(Student stu);

private slots:
    void slotAddData();

    int getFirstPage();
    int getPrePage();
    int getNextPage();
    int getLastPage();
    int getThisPage();
    int getALtogePage(int value);

    void slotOpenFile();
    void slotSaveFile();
    void slotDelRow();
    void slotFileList(QTableWidgetItem *item);
    void slotAddMore();
    void slotChangeTab(int index);
    void slotChangeCombox(int index);
    bool slotEngSex();
    bool slotSex();
    void slotContextMenu(QPoint pos);
    int  slotChangeRow();

    bool isConnectSql();

private:
    Ui::MainWindow *ui;
    QList <Energer> E_list;
    QList <Student> S_list;

    QAction *m_pSaveAction;
    QAction *m_pDelAction;

    QMenu *m_pButtonMenu;
    QMenu *m_pLableMenu;
    QMenu *m_pSqlMenu;

    QButtonGroup *m_pStuSexGroup;
    QButtonGroup *m_pEngSexGroup;

    int m_i_flag;
    int m_i_totalPages;    //总页数
    int m_i_currPage;      //当前页数
    int m_i_stuPage;          //学生总页数
    int m_i_engPage;          //程序员总页数

};
#endif // MAINWINDOW_H
