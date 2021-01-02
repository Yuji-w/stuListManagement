#include "connectDb.h"

/**
 * @brief 数据库查询数据
 * @return
 */
QList<Student> ConnectDb::queryStuDataList()
{
    QList<Student> S_list;
    Student stu;
    QSqlQuery query;   //TODO 添加构造函数中的db对象
    query.exec("SELECT * from student;");
    while(query.next())
    {
        stu.m_s_name = query.value(0).toString();
        stu.m_sex = (Person::ChooseSex)query.value(1).toString().toInt();
        stu.m_i_age = query.value(2).toInt();
        stu.setNum(query.value(3).toInt());
        stu.setPer(query.value(4).toString());
        S_list.append(stu);
    }
    return S_list;
}

/**
 * @brief 数据库连接
 * @return true:打开数据库成功,false:打开数据库失败
 */
bool ConnectDb::isConnectSql()
{
    static QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("123456");
    db.setDatabaseName("stu_manage");
    if (!db.open())
    {
        qDebug()<<db.lastError();
        QMessageBox::critical(NULL, QObject::tr("dont connect"),
                              QObject::tr("error"), QMessageBox::Cancel);
        return false;
    }
    else
    {
        QMessageBox::information(NULL,QObject::tr("success"),
                                 QObject::tr("connect success"),QMessageBox::Cancel);
        qDebug()<<QString("seccess");
        queryStuDataList();
     //   setFormsShow(getThisPage(),getRowVaule());
        return true;
    }
}
/**
 * @brief 数据库添加数据
 * @param stu
 * @return true:添加数据成功，false:添加数据失败
 */
bool ConnectDb::isInsertData(Student stu)
{
    bool ret;;
    QString cmd;
    QSqlQuery query;
    cmd = QString("INSERT INTO student (name,sex,age,num,pre) VALUES(%1,'%2',%3,%4,%5)")
            .arg(stu.m_s_name).arg(stu.m_sex).arg(stu.m_i_age).arg(stu.getNum()).arg(stu.getPer());
    ret = query.exec(cmd);
    if(!ret)
    {
        QMessageBox::critical(NULL, QObject::tr("error"),
                              QObject::tr("addFail"), QMessageBox::Cancel);
        qDebug()<<query.lastError();
    }
    return ret;
}
