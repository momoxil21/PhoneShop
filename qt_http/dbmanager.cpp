#include "dbmanager.h"
#include "xmlmanager.h"


Dbmanager::Dbmanager()
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("127.0.0.1");
    db.setPort(5432);
    db.setDatabaseName("phoneshop");
    db.setUserName("usuario");
    db.setPassword("usuario");
    bool ok = db.open();
    if (!ok)
    {
    }
}


void Dbmanager::newOrder(QString phone, QString repair, QString idorder)
{

    QSqlQuery query;
    query.prepare("INSERT INTO "
                    "orders(statusorders,phoneorders,repairorders,"
                    "orderidorders,dateorders) values('On the way of the technician',"
            "?,?,?,current_timestamp);");

    query.bindValue(0, phone);
    query.bindValue(1, repair);
    query.bindValue(2, idorder);
    query.exec();

    if (!query.lastError().isValid())
    {
        qDebug() << "Error en consulta: " << query.lastError();
    }
}

void Dbmanager::newOrderStatus(QString orderId, QString newStatus)
{

    QSqlQuery query;
    query.prepare("UPDATE orders SET statusorders = ? WHERE orderidorders = ?;");

    query.bindValue(0, newStatus);
    query.bindValue(1, orderId);
    query.exec();

    if (!query.lastError().isValid())
    {
        qDebug() << "Error en consulta: " << query.lastError();
    }
}

QSqlQuery Dbmanager::availableOrders()
{
    QSqlQuery query("SELECT * FROM orders where statusorders != 'done'", db);
    return query;
}

QString Dbmanager::findOrder(QString idorder)
{

    QString result{ "" };
    QSqlQuery query;
    query.prepare("SELECT statusorders FROM orders where orderidorders = ?;");
    query.bindValue(0, idorder);
    query.exec();

    query.next();
    if (query.lastError().isValid())
    {
        result = "This order does not exist";
    }
    else
    {
        result = query.value(0).toString();
    }

    return result;
}

QString Dbmanager::checkLogin(QString username, QString password)
{

    QString result{ "" };
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE iduser = ? AND passworduser = ?");
    query.bindValue(0, username);
    query.bindValue(1, password);
    query.exec();

    query.next();
    if (query.lastError().isValid())
    {
        result = "Error";
    }
    else if (query.numRowsAffected() == 0)
    {
        result = "No";
    }
    else
    {
        result = xmlManager->writeLoginXml(query.value(0).toString());
    }

    return result;
}

void Dbmanager::checkProcessOrders()
{
    int seconds{};
    seconds = 7200;
    QSqlQuery query("SELECT orderidorders,EXTRACT(EPOCH FROM (NOW() - dateorders)),statusorders "
                    "FROM orders WHERE statusorders = 'in process'",
        db);
    while (query.next())
    {
        if (query.lastError().isValid())
        {
            qDebug() << "Error in query checkProcressOrders";
        }
        if (query.value(1).toInt() >= seconds && query.value(2).toString() == "in process")
        {
            QSqlQuery query2("UPDATE orders SET statusorders = 'done' where orderidorders = '"
                    + query.value(0).toString() + "';",
                db);
            if (query2.lastError().isValid())
            {
                qDebug() << "Error in query checkProcressOrders";
            }
        }
    }
}

QSqlQuery Dbmanager::availableRepairs()
{
   QSqlQuery query("SELECT * FROM availablerepairs", db);
   return query;
}
