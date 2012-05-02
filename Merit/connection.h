#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtGui/QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

inline bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/ESRU/Merit/database/sample_uk.db");
    if (!db.open()) {
        QMessageBox::warning(0, QObject::tr("Database Error"),db.lastError().text());
        return false;
    }
    return true;
}

/*
inline void createTables()
{
    QSqlQuery query;
    query.exec("CREATE TABLE artist ("
               "id INTEGER PRIMARY KEY, "
               "name VARCHAR(40) NOT NULL, "
               "country VARCHAR(40))");
    query.exec("CREATE TABLE cd ("
               "id INTEGER PRIMARY KEY, "
               "title VARCHAR(40) NOT NULL, "
               "artistid INTEGER NOT NULL, "
               "year INTEGER NOT NULL, "
               "FOREIGN KEY (artistid) REFERENCES artist)");
    query.exec("CREATE TABLE track ("
               "id INTEGER PRIMARY KEY, "
               "title VARCHAR(40) NOT NULL, "
               "duration INTEGER NOT NULL, "
               "cdid INTEGER NOT NULL)");
}
*/
#endif
