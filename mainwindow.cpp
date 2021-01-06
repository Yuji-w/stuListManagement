#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pageShow.h"
#include "connectDb.h"
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QGridLayout>
#include <QTabBar>
#include <QWidget>
#include <QTabWidget>
#include <QComboBox>
#include <QList>
#include <QFileDialog>
#include <QContextMenuEvent>
#include <QAction>
#include <QCursor>
#include <QDialog>
#include <QDebug>
#include <QCheckBox>
#include <QButtonGroup>

#define DEFAULT_PAGE_NUM 10

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pSaveAction = new QAction(tr("save"),this);  //保存
    m_pDelAction = new QAction(tr("del"),this);    //删除

    m_pButtonMenu = new QMenu(this);

    m_pPageShowStu = new PageShow;
    m_pPageShowEng = new PageShow;
    m_pConnectDb = new ConnectDb;

    m_pPageShowStu->setcurrPage();
    m_pPageShowEng->setcurrPage();

    ui->tableWidget_stu->setRowCount(DEFAULT_PAGE_NUM);
    ui->tableWidget_eng->setRowCount(DEFAULT_PAGE_NUM);
    ui->spinBox_rowCount->setValue(DEFAULT_PAGE_NUM);

    m_pStuSexGroup = new QButtonGroup(this);
    m_pStuSexGroup->addButton(ui->checkBox_man,0);
    m_pStuSexGroup->addButton(ui->checkBox_woman,1);
    ui->checkBox_man->setChecked(true);

    m_pEngSexGroup = new QButtonGroup(this);
    m_pEngSexGroup->addButton(ui->engMan,0);
    m_pEngSexGroup->addButton(ui->engWoman,1);
    ui->engMan->setChecked(true);

    m_i_stuPage = m_pPageShowStu->getThisPage();
    m_i_engPage = m_pPageShowEng->getThisPage();

    chooseRow();
    setRightClickMenu();
    setTabBackGround();

    //添加数据
    connect(ui->addButton,SIGNAL(clicked()),this,SLOT(slotAddData()));

    //连接数据库
    connect(ui->Menu_sql,SIGNAL(triggered()),this,SLOT(slotConnectSql()));

    //点击选项卡切换tab面板
    connect(ui->tabWidget_2,SIGNAL(currentChanged(int)),this,SLOT(slotChangeTab(int)));
    connect(ui->comboBox,SIGNAL(activated(int)),this,SLOT(slotChangeCombox(int)));

    // 切换表格显示行数
    connect(ui->spinBox_rowCount,SIGNAL(editingFinished()),this,SLOT(slotChangeRow()));

    connect(ui->toolButton_change,SIGNAL(clicked()),this,SLOT(slotChangeItem()));


    // 页数操作
    connect(ui->BeginButton,SIGNAL(clicked()),this,SLOT(slotFirstPage()));
    connect(ui->NextButton,SIGNAL(clicked()),this,SLOT(slotNextPage()));
    connect(ui->BackButton,SIGNAL(clicked()),this,SLOT(slotPrePage()));
    connect(ui->LastButton,SIGNAL(clicked()),this,SLOT(slotLastPage()));

    // 文件操作
    connect(ui->OpenButton,SIGNAL(clicked()),this,SLOT(slotOpenFile()));
    connect(ui->pushButton_save,SIGNAL(clicked()),this,SLOT(slotSaveFile()));

    //删除
    connect(ui->delButton,SIGNAL(clicked()),this,SLOT(slotDelRow()));
    connect(ui->toolButton_add,SIGNAL(clicked()),this,SLOT(slotAddMore()));

    connect(ui->tableWidget_stu,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(slotStuFileList(QTableWidgetItem*)));
    connect(ui->tableWidget_eng,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(slotEngFileList(QTableWidgetItem*)));

    // 添加性别
    connect(ui->checkBox_man,SIGNAL(clicked()),this,SLOT(slotSex()));
    connect(ui->checkBox_woman,SIGNAL(clicked()),this,SLOT(slotSex()));
    connect(ui->engMan,SIGNAL(clicked()),this,SLOT(slotEngSex()));
    connect(ui->engWoman,SIGNAL(clicked()),this,SLOT(slotEngSex()));

    // 右键点击事件
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
void MainWindow::slotAddData()
{
    //    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","mysql");
    //    if (QSqlDatabase::contains("mysql"))
    //    {
    //        db = QSqlDatabase::database("mysql");
    //    }
    //    else
    //    {
    //        db = QSqlDatabase::addDatabase("QMYSQL", "mysql");
    //    }
    //根据实际需要创建对象
    if(ui->comboBox->currentIndex() == 0)    //判断当前是否为学生类 0为学生类，1为程序员类
    {
        Student t_stu;
        if(ui->checkBox_man->isChecked() == true)              //获取性别选中状态 true为男性  TODO:性别使用枚举类型
        {
            t_stu.m_sex = (Person::ChooseSex)QString("0").toInt();
        }
        else
        {
            t_stu.m_sex = (Person::ChooseSex)QString("1").toInt();
        }
        t_stu.m_s_name = ui->s_NameEdit->text();
        t_stu.m_i_age = ui->s_AgeEdit->text().toInt();
        t_stu.setNum(ui->s_NumEdit->text().toInt());
        t_stu.setPer(ui->s_PreEdit->text());
        S_list.append(t_stu);    //将数据添加进链表
    }
    else
    {
        Energer t_eng;
        if(ui->engMan->isChecked() == true)
        {
            t_eng.m_sex = (Person::ChooseSex)QString("0").toInt();
        }
        else
        {
            t_eng.m_sex =(Person::ChooseSex)QString("1").toInt();
        }
        t_eng.m_s_name = ui->e_NameEdit->text();    //获取当前填写文字内容
        t_eng.m_i_age = ui->e_AgeEdit->text().toInt();
        t_eng.setWorkAge(ui->e_WorkAgeEdit->text().toInt());
        E_list.append(t_eng);    //将已获取内容添加进链表
    }
    if(ui->comboBox->currentIndex() == 0)              //对学生类操作
    {
        int t_s_len = S_list.size();
        if(m_pPageShowStu->isChangePage(t_s_len,getRowVaule()))        //根据行数判断是否需要翻页
        {
            ui->tableWidget_stu->clearContents();
            m_i_stuPage += 1;
        }
        setFormsShow(m_i_stuPage,getRowVaule());           //在表格显示链表内容
        m_pPageShowStu->getALtogePage(getRowVaule(),t_s_len);  //获取总页数
    }
    else                                                //程序员类操作
    {
        int t_e_len = E_list.size();
        if(m_pPageShowEng->isChangePage(t_e_len,getRowVaule()))
        {
            ui->tableWidget_eng->clearContents();
            m_i_engPage += 1;
        }
        setFormsShow(m_i_engPage,getRowVaule());
        m_pPageShowEng->getALtogePage(getRowVaule(),t_e_len);
    }
    qDebug()<<QString("性别")<<Person::ChooseSex(0)<<QString("性别")<<Person::ChooseSex(1);
}

/**
 * @brief 设置表格背景样式
 */
void MainWindow::setTabBackGround()
{
    ui->tableWidget_stu->setShowGrid(false);  //去除表格边框
    ui->tableWidget_eng->setShowGrid(false);

    ui->tableWidget_stu->setAlternatingRowColors(true);     //隔行换色
    ui->tableWidget_eng->setAlternatingRowColors(true);

    ui->tableWidget_stu->verticalHeader()->setVisible(false);  //隐藏行表头
    ui->tableWidget_eng->verticalHeader()->setVisible(false);

    ui->tableWidget_stu->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  // 表格填充布局,每行等比例显示
    ui->tableWidget_eng->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget_stu->setEditTriggers(QAbstractItemView::NoEditTriggers);   //禁止编辑表格内容
    ui->tableWidget_eng->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

/**
 * @brief 切换选项卡面板
 * @param index
 */
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("openFile"),
                                                    tr("."),
                                                    tr("file(*.txt);;Allfile(*.*)"));
    if(!fileName.isEmpty())         //文件不为空时
    {
        QFile file;
        file.setFileName(fileName);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))  // 打开文件失败
        {
            QMessageBox msg(this);  // 添加this时，将该窗口与MainWindow置于同一对象树
            msg.setText(tr("openFail"));     //打开文件
            msg.exec();
        }
        else
        {
            while (!file.atEnd())
            {
                line = file.readLine();         //读取文件每行数据
                t_lin.append(line);             //将数据存入t_line
            }
            file.close();                       //关闭文件
            t_lin.removeFirst();                //去除表头
            qDebug()<<t_lin.at(1);

            if(ui->comboBox->currentIndex() == 0)
            {
                for(int index = 0;index < t_lin.count();index++)   //将文件内容存入相关链表
                {
                    QString t_line = t_lin.at(index);
                    QStringList lineList = t_line.split(",");
                    t_stu.m_s_name = lineList.at(0);
                    t_stu.m_sex = (Person::ChooseSex)lineList.at(1).toInt();
                    t_stu.m_i_age = lineList.at(2).toInt();
                    t_stu.setNum(lineList.at(3).toInt());
                    t_stu.setPer(lineList.at(4));
                    S_list.append(t_stu);
                }
                m_pPageShowStu->getFirstPage();                 //显示首页
                setFormsShow(m_pPageShowStu->getThisPage(),getRowVaule());
            }
            else
            {
                Energer t_eng;
                for(int index = 0;index < t_lin.count();index++)
                {
                    QString t_line = t_lin.at(index);
                    QStringList lineList = t_line.split(",");
                    t_eng.m_s_name = lineList.at(0);
                    t_eng.m_sex = (Person::ChooseSex)lineList.at(1).toInt();
                    t_eng.m_i_age = lineList.at(2).toInt();
                    t_eng.setWorkAge(lineList.at(3).toInt());
                    E_list.append(t_eng);
                }
                m_pPageShowEng->getFirstPage();
                setFormsShow(m_pPageShowEng->getThisPage(),getRowVaule());
            }
        }
    }
}

/**
 * @brief 选择整行
 */
void MainWindow::chooseRow()
{
    ui->tableWidget_stu->setSelectionBehavior(QAbstractItemView::SelectRows); //点击时选择一行
    ui->tableWidget_eng->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableWidget_stu->setSelectionMode(QAbstractItemView::ExtendedSelection);   //表格填充
    ui->tableWidget_eng->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

/**
 * @brief 删除
 */
void MainWindow::slotDelRow()
{
    if(ui->comboBox->currentIndex() == 0)
    {
        if(informationMessage())                        //是否确认删除
        {
            int t_del = ui->tableWidget_stu->currentRow();   //获取当前选择行
            ui->tableWidget_stu->removeRow(t_del);           //表格删除该数据
            ui->tableWidget_stu->setRowCount(getRowVaule());
            S_list.removeAt(t_del + (m_i_stuPage-1) * getRowVaule());  //删除链表中内容
            if(S_list.size() % getRowVaule() == 0)          //当前页所有数据被删除时，删除后当前表格没有数据，显示上一页内容
            {
                m_i_stuPage = m_i_stuPage - 1;
                setFormsShow(m_i_stuPage,getRowVaule());
                m_pPageShowStu->getALtogePage(getRowVaule(),S_list.size());
            }
        }
    }
    else
    {
        if(informationMessage())
        {
            int t_enu = ui->tableWidget_eng->currentRow();
            ui->tableWidget_eng->removeRow(t_enu);
            ui->tableWidget_eng->setRowCount(getRowVaule());
            E_list.removeAt(t_enu + (m_i_engPage-1)*getRowVaule());
            if(E_list.size() % getRowVaule() == 0)
            {
                m_i_engPage = m_i_engPage - 1;
                setFormsShow(m_i_engPage,getRowVaule());
                m_pPageShowEng->getALtogePage(getRowVaule(),E_list.size());
            }
        }
    }
}

/**
 * @brief 删除警告框
 * @return true: 删除 false: 取消
 */
bool MainWindow::informationMessage()
{
    QString dlgTitle = tr("WORAING");       //警告
    QString strInfo = tr("ISDEL?");         //删除

    QMessageBox::StandardButton result;
    result = QMessageBox::question(this,dlgTitle,strInfo,QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);

    if (result == QMessageBox::Yes)   //判断点击结果，确认返回true
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
    m_pButtonMenu->addAction(m_pSaveAction);    //保存菜单
    m_pButtonMenu->addAction(m_pDelAction);     //删除菜单

    ui->tableWidget_stu->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidget_eng->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableWidget_stu, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenu(QPoint)));
    connect(ui->tableWidget_eng, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenu(QPoint)));
}

/**
 * @brief 获取行数
 * @return
 */
int MainWindow::getRowVaule()
{
    return ui->spinBox_rowCount->value();
}

/**
 * @brief 文本菜单
 * @param pos
 */
void MainWindow::slotContextMenu(QPoint pos)
{
    if(ui->comboBox->currentIndex() == 0)
    {
        auto index = ui->tableWidget_stu->indexAt(pos);  //鼠标点击焦点
        if (index.isValid())
        {
            m_pButtonMenu->exec(QCursor::pos());     //菜单栏
        }
    }
    else
    {
        auto index = ui->tableWidget_eng->indexAt(pos);
        if (index.isValid())
        {
            m_pButtonMenu->exec(QCursor::pos());
        }
    }
}

/**
 * @brief 修改每页显示条目数
 * @return
 */
int MainWindow::slotChangeRow()
{
    int m_i_cgRow = ui->spinBox_rowCount->value();             //获取spin_box值
    if(ui->comboBox->currentIndex() == 0)                       //学生类操作
    {
        ui->tableWidget_stu->setRowCount(m_i_cgRow);                //设置表格行数
        ui->tableWidget_stu->clearContents();                       //刷新表格
        m_pPageShowStu->getLastPage(getRowVaule(),S_list.size());
        ui->tableWidget_stu->clearContents();
        setFormsShow(m_pPageShowStu->getThisPage(),getRowVaule());
    }
    else
    {
        ui->tableWidget_eng->setRowCount(m_i_cgRow);
        ui->tableWidget_eng->clearContents();
        m_pPageShowEng->getLastPage(getRowVaule(),E_list.size());
        ui->tableWidget_eng->clearContents();
        setFormsShow(m_pPageShowEng->getThisPage(),getRowVaule());
    }
    return m_i_cgRow;
}

/**
 * @brief 下一页
 */
void MainWindow::slotNextPage()
{
    int t_i_page = 0;
    if(ui->comboBox->currentIndex() == 0)
    {
        t_i_page = m_pPageShowStu->getNextPage(getRowVaule(),S_list.size());
        ui->tableWidget_stu->clearContents();
    }
    else
    {
        t_i_page = m_pPageShowEng->getNextPage(getRowVaule(),E_list.size());
        ui->tableWidget_eng->clearContents();
    }
    setFormsShow(t_i_page,getRowVaule());
}

/**
 * @brief 首页
 */
void MainWindow::slotFirstPage()
{
    int t_i_page = 0;
    if(ui->comboBox->currentIndex() == 0)
    {
        t_i_page = m_pPageShowStu->getFirstPage();
    }
    else
    {
        t_i_page = m_pPageShowEng->getFirstPage();
    }
    setFormsShow(t_i_page,getRowVaule());
}

/**
 * @brief 上一页
 */
void MainWindow::slotPrePage()
{
    int t_i_page = 0;
    if(ui->comboBox->currentIndex() == 0)
    {
        t_i_page = m_pPageShowStu->getPrePage();
    }
    else
    {
        t_i_page = m_pPageShowEng->getPrePage();
    }
    setFormsShow(t_i_page,getRowVaule());
}

/**
 * @brief 末页
 */
void MainWindow::slotLastPage()
{
    int t_i_page = 0;
    if(ui->comboBox->currentIndex() == 0)
    {
        t_i_page = m_pPageShowStu->getLastPage(getRowVaule(),S_list.size());
        ui->tableWidget_stu->clearContents();
    }
    else
    {
        t_i_page = m_pPageShowEng->getLastPage(getRowVaule(),E_list.size());
        ui->tableWidget_eng->clearContents();
    }
    setFormsShow(t_i_page - 1,getRowVaule());
}

/**
 * @brief 连接数据库
 */
void MainWindow::slotConnectSql()
{
    bool t_isCon = m_pConnectDb->isConnectSql();
    if(t_isCon)
    {
        m_pConnectDb->queryStuDataList();
        setFormsShow(m_pPageShowStu->getThisPage(),getRowVaule());
    }
}

/**
 * @brief 输入框显示表格数据
 * @param item
 */
void MainWindow::slotStuFileList(QTableWidgetItem *item)   //学生类
{
    int itemRow = item->row();

    if(ui->comboBox->currentIndex() == 0)
    {
        Student Sli;
        Sli.m_s_name = ui->tableWidget_stu->item(itemRow,0)->text();
        Sli.m_i_age = ui->tableWidget_stu->item(itemRow,2)->text().toInt();

        int t_num = Sli.getNum();
        QString pre = Sli.getPer();
        t_num = ui->tableWidget_stu->item(itemRow,3)->text().toInt();
        pre = ui->tableWidget_stu->item(itemRow,4)->text();
        QString str = ui->tableWidget_stu->item(itemRow,1)->text();

        if(str == tr("男"))
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
        ui->s_NumEdit->setText(QString::number(t_num));
    }
}

void MainWindow::slotEngFileList(QTableWidgetItem *item)   //程序员类
{
    int itemRow = item->row();
    Energer Eli;
    Eli.m_s_name = ui->tableWidget_eng->item(itemRow,0)->text();
    Eli.m_i_age = ui->tableWidget_eng->item(itemRow,2)->text().toInt();

    int t_age = Eli.getWorkAge();
    t_age = ui->tableWidget_eng->item(itemRow,3)->text().toInt();
    QString str = ui->tableWidget_eng->item(itemRow,1)->text();

    if(str == tr("男"))
    {
        ui->engMan->setChecked(true);
    }
    else
    {
        ui->engWoman->setChecked(true);
    }
    ui->e_NameEdit->setText(Eli.m_s_name);
    ui->e_AgeEdit->setText(QString::number(Eli.m_i_age));
    ui->e_WorkAgeEdit->setText(QString::number(t_age));
}

/**
 * @brief 批量添加
 */
void MainWindow::slotAddMore()
{
    int t_number = ui->lineEdit_add->text().toInt();
    for (int i = 0;i < t_number;i++)
    {
        slotAddData();
    }
}

void MainWindow::slotChangeItem()
{
    qDebug()<<QString("aaaaaaaaa");
    if(ui->comboBox->currentIndex() == 0)
    {
        Student sli;
        sli.m_s_name = ui->e_NameEdit->text();
        if(ui->checkBox_man->isChecked() == true)
        {
            sli.m_sex = (Person::ChooseSex)QString("0").toInt();
        }
        else
        {
            sli.m_sex = (Person::ChooseSex)QString("1").toInt();
        }
        sli.m_i_age = ui->s_AgeEdit->text().toInt();
        sli.setNum(ui->s_NumEdit->text().toInt());
        sli.setPer(ui->s_PreEdit->text());
        S_list.replace(getRowVaule()*m_pPageShowStu->getPrePage() + ui->tableWidget_stu->currentRow(),sli);
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

void MainWindow::slotChangeCombox(int index)
{
    ui->tabWidget_2->setCurrentIndex(index);
    ui->stackedWidget->setCurrentIndex(index);
}

/**
 * @brief 保存文件到本地
 */
//TODO 只能保存一份
void MainWindow::slotSaveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "C:/Users/Yuji/Desktop/导出文件测试/save.txt",
                                                    tr("Images (*.txt)"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))  //文件打开失败
    {
        return;
    }
    else
    {
        if(ui->comboBox->currentIndex() == 0)   //判断当前是否为学生类
        {
            QTextStream out(&file);
            QString t_item;
            QString t_head = (tr("name,sex,age,num,pre"));   //设置表头    姓名，性别，年龄，学号，专业
            out << t_head<< "\n";
            for (int i = 0; i<ui->tableWidget_stu->columnCount(); i++)
            {
                int t_row = ui->tableWidget_stu->currentRow();  //获取当前选中行
                t_item = ui->tableWidget_stu->item(t_row,i)->text();
                out << t_item <<QString(",")<<QString("    ");
            }
        }
        else
        {
            QTextStream out(&file);
            QString t_item;
            QString t_head = (tr("name,sex,age,workAge"));  //姓名，性别，年龄，工龄
            out << t_head<< "\n";
            for (int i = 0; i<ui->tableWidget_eng->columnCount(); i++)
            {
                int t_row = ui->tableWidget_eng->currentRow();
                t_item = ui->tableWidget_eng->item(t_row,i)->text();
                out << t_item <<QString(",")<<QString("    ");
            }
        }
    }
}

/**
 * @brief 性别添加
 * @return true: 男性 false: 女性
 */
Person::ChooseSex MainWindow::slotSex()
{
    switch (m_pStuSexGroup->checkedId())  //判断性别，1为男性，0为女性
    {
    case 0:
        return Student::man;
    case 1:
        return Student::woman;
    }

}

/**
 * @brief 程序员类性别添加
 * @return true: 男性 false: 女性
 */
Person::ChooseSex MainWindow::slotEngSex()
{
    switch (m_pEngSexGroup->checkedId())
    {
    case 0:
        return Energer::man;
        break;
    case 1:
        return Energer::woman;
        break;
    }
}

/**
 * @brief 表格列表显示文字
 * @param 当前页
 * @param 当前表格条目数
 */
void MainWindow::setFormsShow(int page,int t_row)
{
    qDebug()<<page;
    QString str;
    if(ui->comboBox->currentIndex() == 0)  //判断是否为学生类
    {
        str = (QString("%1/%2")).arg(page + 1).arg(m_pPageShowStu->getALtogePage(t_row,S_list.size()));  //底部显示总页数及当前页数
    }
    else
    {
        str = (QString("%1/%2")).arg(page + 1).arg(m_pPageShowEng->getALtogePage(t_row,E_list.size()));
    }
    ui->label_page->setText(str);
    ui->tableWidget_stu->setRowCount(t_row);
    ui->tableWidget_eng->setRowCount(t_row);
    for(int row = 0;row <= t_row - 1;row++)
    {
        switch (ui->comboBox->currentIndex())
        {
        case 0:     //当前为学生类时
        {
            if(row < S_list.size())
            {
                Student a = S_list.at(row + (t_row * page));
                for (int i = 0;i <= ui->tableWidget_stu->columnCount();i++)
                {
                    QTableWidgetItem *item = new QTableWidgetItem;
                    switch (i)              //表格与链表值对应
                    {
                    case 0:
                        item->setText(QString(a.m_s_name));
                        break;
                    case 1:
                        if(a.m_sex == Student::man)
                        {
                            item->setText(QString("男"));
                        }
                        else
                        {
                            item->setText(QString("女"));
                        }
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
                    ui->tableWidget_stu->setItem(row,i,item);
                }
            }
            if(row+t_row*page + 1 == S_list.size())  //链表数据添加完毕时
            {
                return;
            }
            break;
        }
        case 1:             //当前为程序员类时
        {
            if (row < E_list.size())
            {
                Energer b = E_list.at(row + (t_row * page));
                for (int i = 0;i <= ui->tableWidget_eng->columnCount();i++)
                {
                    QTableWidgetItem *item = new QTableWidgetItem;
                    switch (i)
                    {
                    case 0:
                        item->setText(QString(b.m_s_name));
                        break;
                    case 1:
                        if(b.m_sex == 0)
                        {
                            item->setText(QString("男"));
                        }
                        else
                        {
                            item->setText(QString("女"));
                        }
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
                    ui->tableWidget_eng->setItem(row,i,item);
                }
            }
            if(row+t_row*page + 1 == E_list.size())
            {
                return;
            }
        }
        }
    }
}
