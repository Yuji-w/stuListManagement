#ifndef CONNECTDB_H
#define CONNECTDB_H

#include "mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>

class Student;
class ConnectDb
{
public:
    explicit ConnectDb()
    {
    }
    ~ConnectDb();

public:
    bool isConnectSql();
    QList<Student> queryStuDataList();
    bool isInsertData(Student stu);

private:
    Student *m_pStudent;
};

#endif // CONNECTDB_H
