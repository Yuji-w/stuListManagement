#ifndef CONNECTDB_H
#define CONNECTDB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>

bool connectData(){
    {
        qDebug()<<QString("11");
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setUserName("root");
        db.setPassword("123123");
        db.setDatabaseName("stumanagement");
        if (!db.open()) {
            QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
                                  QObject::tr("Unable to establish a database connection"), QMessageBox::Cancel);
            return false;
        }
    }
}






#endif // CONNECTDB_H
