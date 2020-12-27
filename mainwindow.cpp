#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QGridLayout>
#include <QTabBar>
#include <QWidget>
#include <QTabWidget>
#include <QComboBox>
#include <QList>
#include <QTextCodec>
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <QContextMenuEvent>
#include <QAction>
#include <QCursor>
#include <QMessageBox>
#include <QDialog>
#include <QDebug>
#include <QCheckBox>
#include <QButtonGroup>
#include <QDesktopServices>
#include <QUrl>
#include <QAxObject>
#include <QDateTime>
#include <iostream>

#define DEFAULT_PAGE_NUM 10

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pSaveAction = new QAction(tr("保存"),this);
    m_pDelAction = new QAction(tr("删除"),this);

    m_pButtonMenu = new QMenu(this);
    m_pLableMenu = new QMenu(this);

    m_i_currPage = 0;

    ui->tableWidget->setRowCount(DEFAULT_PAGE_NUM);
    ui->tableWidget_2->setRowCount(DEFAULT_PAGE_NUM);
    ui->spinBox_rowCount->setValue(DEFAULT_PAGE_NUM);

    m_pStuSexGroup = new QButtonGroup(this);
    m_pStuSexGroup->addButton(ui->checkBox_man,0);
    m_pStuSexGroup->addButton(ui->checkBox_woman,1);
    ui->checkBox_man->setChecked(true);

    m_pEngSexGroup = new QButtonGroup(this);
    m_pEngSexGroup->addButton(ui->engMan,0);
    m_pEngSexGroup->addButton(ui->engWoman,1);
    ui->engMan->setChecked(true);

    chooseRow();
    setRightClickMenu();
    setTabBackGround();

    /**
     * @brief 添加数据
     */
    connect(ui->addButton,SIGNAL(clicked()),this,SLOT(addData()));

    /**
     * @brief 点击选项卡切换tab面板
     */
    connect(ui->tabWidget_2,SIGNAL(currentChanged(int)),this,SLOT(slotChangeTab(int)));
    connect(ui->spinBox_rowCount,SIGNAL(editingFinished()),this,SLOT(slotChangeRow()));

    /**
     * @brief 页数操作
     */
    connect(ui->BeginButton,SIGNAL(clicked()),this,SLOT(getFirstPage()));
    connect(ui->NextButton,SIGNAL(clicked()),this,SLOT(getNextPage()));
    connect(ui->BackButton,SIGNAL(clicked()),this,SLOT(getPrePage()));
    connect(ui->LastButton,SIGNAL(clicked()),this,SLOT(getLastPage()));

    connect(ui->OpenButton,SIGNAL(clicked()),this,SLOT(slotOpenFile()));
    connect(ui->pushButton_save,SIGNAL(clicked()),this,SLOT(slotSaveFile()));

    connect(ui->delButton,SIGNAL(clicked()),this,SLOT(slotDelRow()));
    connect(ui->toolButton_add,SIGNAL(clicked()),this,SLOT(slotMoreAdd()));

    connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(slotFileList(QTableWidgetItem*)));

    /**
     * @brief 添加性别
     */
    connect(ui->checkBox_man,SIGNAL(clicked()),this,SLOT(slotSex()));
    connect(ui->checkBox_woman,SIGNAL(clicked()),this,SLOT(slotSex()));
    connect(ui->engMan,SIGNAL(clicked()),this,SLOT(slotEngSex()));
    connect(ui->engWoman,SIGNAL(clicked()),this,SLOT(slotEngSex()));

    /**
     * @brief 右键点击事件
     */
    connect(m_pSaveAction,SIGNAL(triggered()),this,SLOT(slotSaveFile()));
    connect(m_pDelAction,SIGNAL(triggered()),this,SLOT(slotDelRow()));
};

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief 将添加数据添加至链表
 */
void MainWindow::addData()
{
    qDebug()<<QString("error");
    //根据实际需要创建对象
    if(ui->comboBox->currentIndex() == 1)
    {
        Energer t_eng;
        if(slotEngSex() == 1)
        {
            t_eng.m_s_sex = ui->engMan->text();
        }
        else
        {
            t_eng.m_s_sex = ui->engWoman->text();
        }
        t_eng.m_s_name = ui->e_NameEdit->text();
        t_eng.m_i_age = ui->e_AgeEdit->text().toInt();
        t_eng.setWorkAge(ui->e_WorkAgeEdit->text().toInt());
        E_list.append(t_eng);
    }
    else
    {
        Student t_stu;
        if(slotSex() == 1)
        {
            t_stu.m_s_sex = ui->checkBox_man->text();
        }
        else
        {
            t_stu.m_s_sex = ui->checkBox_woman->text();
        }
        t_stu.m_s_name = ui->s_NameEdit->text();
        t_stu.m_i_age = ui->s_AgeEdit->text().toInt();
        t_stu.setNum(ui->s_NumEdit->text().toInt());
        t_stu.setPer(ui->s_PreEdit->text());
        S_list.append(t_stu);
    }

    int t_e_len = E_list.size();
    int t_s_len = S_list.size();
    if(ui->comboBox->currentIndex()==0)
    {
        if(isChangePage(t_s_len,getRowVaule()))
        {
            ui->tableWidget->clearContents();
            m_i_currPage += 1;
        }
        setFormsShow(m_i_currPage,getRowVaule());
        getALtogePage(getRowVaule());
    }
    else
    {
        if(isChangePage(t_e_len,getRowVaule()))
        {
            ui->tableWidget_2->clearContents();
            m_i_currPage += 1;
        }
        setFormsShow(m_i_currPage,getRowVaule());
        getALtogePage(getRowVaule());
    }
}

/**
 * @brief 判断是否换行
 * @param i_data
 * @return
 */
bool MainWindow::isChangePage(int listLenth,int rowValue)
{
    if( listLenth % rowValue ==1 && listLenth != 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief 表格样式
 */
void MainWindow::setTabBackGround()
{
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget_2->setShowGrid(false);

    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget_2->setAlternatingRowColors(true);

    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget_2->verticalHeader()->setVisible(false);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::changeComboBox(int index)
{
    ui->tabWidget_2->setCurrentIndex(index);
    ui->stackedWidget->setCurrentIndex(index);
}

/**
 * @brief 导入文件
 */
void MainWindow::slotOpenFile()
{
    QStringList t_lin;
    Student t_stu;
    QString line;
    QString fileName = QFileDialog::getOpenFileName(this, tr("导入文件"),
                                                    tr("."),
                                                    tr("file(*.txt);;Allfile(*.*)"));
    if(!fileName.isEmpty())
    {
        QFile file;
        file.setFileName(fileName);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox msg;
            msg.setText("打开文件失败!");
            msg.exec();
        }
        else
        {
            while (!file.atEnd())
            {
                line = file.readLine();
                t_lin.append(line);
            }
            file.close();

            qDebug()<<t_lin;
            t_lin.removeFirst();
            qDebug()<<t_lin.at(1);

            if(ui->comboBox->currentIndex() == 0)
            {
            for(int index=0;index<t_lin.count();index++)
            {
                QString t_line = t_lin.at(index);
                QStringList lineList = t_line.split(",");

                t_stu.m_s_name = lineList.at(0);
                t_stu.m_s_sex = lineList.at(1);
                t_stu.m_i_age = lineList.at(2).toInt();
                t_stu.setNum(lineList.at(3).toInt());
                t_stu.setPer(lineList.at(4));
                S_list.append(t_stu);
            }
            }
            else
            {
                Energer t_eng;
                for(int index=0;index<t_lin.count();index++)
                {
                    QString t_line = t_lin.at(index);
                    QStringList lineList = t_line.split(",");
                    t_eng.m_s_name = lineList.at(0);
                    t_eng.m_s_sex = lineList.at(1);
                    t_eng.m_i_age = lineList.at(2).toInt();
                    t_eng.setWorkAge(lineList.at(3).toInt());
                    E_list.append(t_eng);
                }
            }
            getFirstPage();
        }
    }
}

/**
 * @brief 选择整行
 */
void MainWindow::chooseRow()
{
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidget_2->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

/**
 * @brief 删除
 */
void MainWindow::slotDelRow()
{
    int t_row = ui->spinBox_rowCount->value();
    getThisPage();
    if(ui->comboBox->currentIndex()==0)
    {
        if(informationMessage())
        {
            int t_del = ui->tableWidget->currentRow();
            ui->tableWidget->removeRow(t_del);
            ui->tableWidget->setRowCount(t_row);
            S_list.removeAt(t_del+(m_i_currPage-1)*t_row);
            if(S_list.size() % t_row == 0)
            {
                m_i_currPage = m_i_currPage - 1;
                setFormsShow(m_i_currPage,getRowVaule());
                getALtogePage(getRowVaule());
            }
        }
    }
    else
    {
        if(informationMessage())
        {
            int t_enu = ui->tableWidget_2->currentRow();
            ui->tableWidget_2->removeRow(t_enu);
            ui->tableWidget_2->setRowCount(t_row);
            E_list.removeAt(t_enu+(m_i_currPage-1)*t_row);
            if(E_list.size() % t_row == 0)
            {
                m_i_currPage = m_i_currPage - 1;
                setFormsShow(m_i_currPage,getRowVaule());
                getALtogePage(getRowVaule());
            }
        }
    }
}

/**
 * @brief 删除警告框
 * @return
 */
bool MainWindow::informationMessage()
{
    QString dlgTitle = tr("警告");
    QString strInfo = tr("是否删除");

    QMessageBox::StandardButton result;
    result = QMessageBox::question(this,dlgTitle,strInfo,QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);

    if (result == QMessageBox::Yes)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief 右键菜单
 */
void MainWindow::setRightClickMenu()
{
    m_pButtonMenu->addAction(m_pSaveAction);
    m_pButtonMenu->addAction(m_pDelAction);

    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenu(QPoint)));
    connect(ui->tableWidget_2, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenu(QPoint)));
}

int MainWindow::getRowVaule()
{
    return ui->spinBox_rowCount->value();
}

void MainWindow::slotContextMenu(QPoint pos)
{
    if(ui->comboBox->currentIndex()==0)
    {
        auto index = ui->tableWidget->indexAt(pos);
        if (index.isValid())
        {
            m_pButtonMenu->exec(QCursor::pos());
        }
    }
    else
    {
        auto index = ui->tableWidget_2->indexAt(pos);
        if (index.isValid())
        {
            m_pButtonMenu->exec(QCursor::pos());
        }
    }
}

int MainWindow::slotChangeRow()
{
    int m_i_cgRow = ui->spinBox_rowCount->value();
    ui->tableWidget->setRowCount(m_i_cgRow);
    ui->tableWidget_2->setRowCount(m_i_cgRow);
    ui->tableWidget->clearContents();
    getLastPage();
    return m_i_cgRow;
}

/**
 * @brief 输入框显示表格数据
 * @param item
 */
void MainWindow::slotFileList(QTableWidgetItem *item)
{
    int itemRow = item->row();

    Student Sli;
    Sli.m_s_name =  ui->tableWidget->item(itemRow,0)->text();
    Sli.m_i_age = ui->tableWidget->item(itemRow,2)->text().toInt();

    int num = Sli.getNum();
    QString pre = Sli.getPer();

    num = ui->tableWidget->item(itemRow,3)->text().toInt();
    pre = ui->tableWidget->item(itemRow,4)->text();
    Sli.m_s_sex = ui->tableWidget->item(itemRow,1)->text();

    if(Sli.m_s_sex == tr("男"))
    {
        ui->checkBox_man->setChecked(true);
    }
    else
    {
        ui->checkBox_woman->setChecked(true);
    }

    ui->s_NameEdit->setText(Sli.m_s_name);
    ui->s_AgeEdit->setText(QString::number(Sli.m_i_age));
    ui->s_PreEdit->setText(pre);
    ui->s_NumEdit->setText(QString::number(num));
}

/**
 * @brief 批量添加
 */
void MainWindow::slotMoreAdd()
{
    int t_number = ui->lineEdit_add->text().toInt();
    for (int i = 0;i < t_number;i++)
    {
        addData();
    }
}

/**
 * @brief 切换选项卡
 * @param index
 */
void MainWindow::slotChangeTab(int index)
{
    ui->comboBox->setCurrentIndex(index);
    ui->stackedWidget->setCurrentIndex(index);
}

/**
 * @brief 保存文件到本地
 */
//TODO 只能保存一份
void MainWindow::slotSaveFile()
{
    QString str;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "C:/Users/Yuji/Desktop/导出文件测试/save.txt",
                                                    tr("Images (*.txt)"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    else
    {
        if(ui->comboBox->currentIndex() == 0)
        {
        QTextStream out(&file);
        QString t_item;
        QString t_head=("姓名,性别,年龄,学号,专业");
        out << t_head<< "\n";
        for (int i = 0; i<ui->tableWidget->columnCount(); i++)
        {
            int t_row = ui->tableWidget->currentRow();
            t_item = ui->tableWidget->item(t_row,i)->text();
            out << t_item <<QString(",")<<QString("    ");
        }
        }
        else
        {
            QTextStream out(&file);
            QString t_item;
            QString t_head=("姓名,性别,年龄,工龄");
            out << t_head<< "\n";
            for (int i = 0; i<ui->tableWidget_2->columnCount(); i++)
            {
                int t_row = ui->tableWidget_2->currentRow();
                t_item = ui->tableWidget_2->item(t_row,i)->text();
                out << t_item <<QString(",")<<QString("    ");
            }
        }
    }
}


/**
 * @brief 获取当前页数
 * @return
 */
int MainWindow::getThisPage()
{
    return m_i_currPage;
}

/**
 * @brief 获取总页数
 */
int MainWindow::getALtogePage(int t_row)
{
    if(ui->comboBox->currentIndex() == 0)
    {
        if(S_list.size() % t_row != 0)
        {
            m_i_stuPage = S_list.size() / t_row+1;
        }
        else
        {
            m_i_stuPage = S_list.size() / t_row;
        }
        return m_i_stuPage;
    }
    else
    {
        if(E_list.size() % t_row != 0)
        {
            m_i_engPage = E_list.size() / t_row+1;
        }
        else
        {
            m_i_engPage = E_list.size() / t_row;
        }
        return  m_i_engPage;
    }

}

/**
 * @brief 获取上一页
 * @return
 */
int MainWindow::getPrePage()
{
    if(m_i_currPage==0)
    {
        m_i_currPage=0;
    }
    else
    {
        m_i_currPage = m_i_currPage-1;
    }
    setFormsShow(m_i_currPage,getRowVaule());
    return m_i_currPage;
}

/**
 * @brief 获取下一页
 * @return
 */
int MainWindow::getNextPage()
{
    getALtogePage(getRowVaule());
    if(ui->comboBox->currentIndex()==0)
    {
        if(m_i_currPage==m_i_stuPage-1)
        {
            return m_i_currPage;
        }
        else
        {
            m_i_currPage+=1;
            ui->tableWidget->clearContents();
            setFormsShow(m_i_currPage,getRowVaule());
            qDebug()<<(m_i_currPage);
        }
    }
    else
    {
        if(m_i_currPage==m_i_engPage-1)
        {
            return m_i_currPage;
        }
        else
        {
            m_i_currPage+=1;
            ui->tableWidget_2->clearContents();
            setFormsShow(m_i_currPage,getRowVaule());
        }
    }
    return m_i_currPage;
}

/**
 * @brief 获取首页
 * @return
 */
int MainWindow::getFirstPage()
{
    m_i_currPage=0;
    setFormsShow(m_i_currPage,getRowVaule());
    qDebug()<<(m_i_currPage);
    return  m_i_currPage;
}

/**
 * @brief 获取尾页
 * @return
 */
int MainWindow::getLastPage()
{
    getALtogePage(getRowVaule());
    int sTotal = m_i_stuPage-1;
    if(ui->comboBox->currentIndex()==0)
    {
        ui->tableWidget->clearContents();
        setFormsShow(sTotal,getRowVaule());
        m_i_currPage = sTotal;
        return m_i_stuPage;
    }
    else
    {
        int e_Total = m_i_engPage-1;
        ui->tableWidget_2->clearContents();
        setFormsShow(e_Total,getRowVaule());
        m_i_currPage = e_Total;
        return m_i_engPage;
    }
}

/**
 * @brief 性别添加
 * @return
 */
bool MainWindow::slotSex()
{
    switch (m_pStuSexGroup->checkedId())
    {
    case 0:
        return 1;
        break;
    case 1:
        return 0;
        break;
    }
    return 0;
}

bool MainWindow::slotEngSex()
{
    switch (m_pEngSexGroup->checkedId())
    {
    case 0:
        return 1;
        break;
    case 1:
        return 0;
        break;
    }
    return 0;
}
/**
 * @brief 表格列表显示文字
 * @param page
 */
void MainWindow::setFormsShow(int page,int t_row)
{
    QString str = (QString("%1/%2")).arg(page+1).arg(getALtogePage(t_row));
    ui->label_page->setText(str);
    ui->tableWidget->setRowCount(t_row);
    ui->tableWidget_2->setRowCount(t_row);
    for(int row=0;row<=t_row-1;row++)
    {
        switch (ui->comboBox->currentIndex())
        {
        case 1:
        {
            if (row<E_list.size())
            {
                Energer b = E_list.at((row+t_row*page));
                for (int i=0;i<=ui->tableWidget_2->columnCount();i++)
                {
                    QTableWidgetItem *item = new QTableWidgetItem;
                    switch (i)
                    {
                    case 0:
                        item->setText(QString(b.m_s_name));
                        break;
                    case 1:
                        item->setText(QString(b.m_s_sex));
                        break;
                    case 2:
                        item->setText(QString::number(b.m_i_age));
                        break;
                    case 3:
                        item->setText(QString::number(b.getWorkAge()));
                        break;
                    default:
                        break;
                    }
                    ui->tableWidget_2->setItem(row,i,item);
                }
            }
            if(row+t_row*page+1==E_list.size())
            {
                return;
            }
        }

        case 0:
        {
            if(row<S_list.size())
            {
                Student a = S_list.at(row+(t_row*page));
                for (int i=0;i<=ui->tableWidget->columnCount();i++)
                {
                    QTableWidgetItem *item = new QTableWidgetItem;
                    switch (i)
                    {
                    case 0:
                        item->setText(QString(a.m_s_name));
                        break;
                    case 1:
                        item->setText(QString(a.m_s_sex));
                        break;
                    case 2:
                        item->setText(QString::number(a.m_i_age));
                        break;
                    case 3:
                        item->setText(QString::number(a.getNum()));
                        break;
                    case 4:
                        item->setText(QString(a.getPer()));
                    default:
                        break;
                    }
                    ui->tableWidget->setItem(row,i,item);
                }
            }
            if(row+t_row*page+1==S_list.size())
            {
                return;
            }
        }
        }
    }
}
