#ifndef XMLMANAGER_H
#define XMLMANAGER_H

#include <iostream>
#include <QString>
#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
#include <QtXml>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlSchema>
#include <QtXmlPatterns/QXmlSchema>
#include <QtXmlPatterns/QXmlSchemaValidator>
#include <QSqlQuery>
#include <QSql>
#include <QTemporaryFile>
#include <QXmlStreamReader>
#include <QDir>

class Xmlmanager
{
public:
    Xmlmanager();
    std::tuple<QString,QString, QString> readNewOrder();
    QString readFindOrder();
    std::tuple<QString, QString> readLogin();
    QString writeOrderStatusXml(QString orderStatus);
    QString writeLoginXml(QString orderStatus);
    QString writeOrdersXml(QSqlQuery query);
    bool validatexml(QString xml, QString xsd);
    QString xmlMessage(QString xml);
    std::tuple<QString, QString> readNewOrderStatus(QString xml);
    QString makeFiles(QString fileName, QString message);
    QString writeNewOrderStatus(QString idorder, QString newStatus);


private:
    QDomDocument newOrderXML;
    QDomDocument findOrderXML;
    QDomDocument LoginXML;
    QDomDocument OrdersXML;
    QDomDocument newOrderStatusXML;
};

#endif // XMLMANAGER_H
