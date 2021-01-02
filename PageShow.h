#ifndef PAGESHOW_H
#define PAGESHOW_H
#include "mainwindow.h"

#include <QMainWindow>
#include <QObject>

/**
 * @brief 分页算法
 * 总共的条目数
 * 每页的条数
 * 当前页
 * 当前页起始的条目
 * 当前页结束的条目
 * fun 上一页
 * fun 下一页
 */
class PageShow : public QObject
{
    Q_OBJECT
public:
    explicit PageShow(){

    };
    ~PageShow();

signals:

public:
    void setTotalNum();
    int getCurrPage();
    void setcurrPage();
    bool isChangePage(int listLenth,int rowValue);

public slots:
     int getFirstPage();
     int getPrePage();
     int getNextPage(int value,int listlen);
     int getLastPage(int value,int listlen);
     int getThisPage();
     int getALtogePage(int value,int listlen);

private:
     int m_i_flag;
     int m_i_totalPages;    //总页数
     int m_i_currPage;      //当前页数
     int m_i_tolpage;

private slots:

};

#endif // PAGESHOW_H
