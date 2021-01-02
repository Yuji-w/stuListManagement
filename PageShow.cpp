#include "pageShow.h"
#include "ui_mainwindow.h"
#include <QDebug>

/**
 * @brief 获取当前页数
 * @return
 */
int PageShow::getThisPage()
{
    return m_i_currPage;
}

/**
 * @brief 获取总页数
 */
int PageShow::getALtogePage(int value,int listlen)
{
    if(listlen % value != 0)
    {
        m_i_totalPages = listlen / value + 1;
    }
    else
    {
        m_i_totalPages = listlen / value;
    }
    return  m_i_totalPages;
}

/**
 * @brief 获取上一页
 * @return
 */
int PageShow::getPrePage()
{
    if(m_i_currPage == 0)
    {
        m_i_currPage = 0;
    }
    else
    {
        m_i_currPage = m_i_currPage - 1;
    }
    return m_i_currPage;
}

/**
 * @brief 获取下一页
 * @return
 */
int PageShow::getNextPage(int value,int listlen)
{
    getALtogePage(value,listlen);
    if(m_i_currPage == m_i_totalPages - 1)
    {
        return m_i_currPage;
    }
    else
    {
        m_i_currPage += 1;
        qDebug()<<(m_i_currPage);
    }
    return m_i_currPage;
}

/**
 * @brief 获取首页
 * @return
 */
PageShow::~PageShow()
{

}

void PageShow::setcurrPage()
{
    m_i_currPage = 0;
}

/**
 * @brief 判断是否需要翻页
 * @param listLenth  链表长度
 * @param rowValue 当前表格行数
 * @return true: 翻页 false:不翻页
 */
bool PageShow::isChangePage(int listLenth,int rowValue)
{
    if( listLenth % rowValue == 1 && listLenth != 1)  //当前链表长度 = 当前行数 + 1 时翻页
    {
        return true;
    }
    else
    {
        return false;
    }
}

int PageShow::getFirstPage()
{
    //TODO 修改格式
    m_i_currPage = 0;
    qDebug()<<(m_i_currPage);
    return  m_i_currPage;
}

/**
 * @brief 获取尾页
 * @return
 */
int PageShow::getLastPage(int value,int listlen)
{
    getALtogePage(value,listlen);
    int sTotal = m_i_totalPages - 1;
    m_i_currPage = sTotal;
    return m_i_totalPages;
}
