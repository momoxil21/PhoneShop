#include "testserver.h"


TestServer::TestServer(quint16 port)
    : m_pWebSocketServer(new QWebSocketServer(
          QStringLiteral("Test Server"), QWebSocketServer::NonSecureMode, this))
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        qDebug() << "Server iniciado en puerto:" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this,
            &TestServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &TestServer::closed);
    } // end if
}

TestServer::~TestServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}


void TestServer::onNewConnection()
{
    QWebSocket* pSocket = m_pWebSocketServer->nextPendingConnection();
    qDebug() << "Socket conectado:" << pSocket;

    connect(pSocket, &QWebSocket::textMessageReceived, this, &TestServer::processTextMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &TestServer::socketDisconnected);

    m_clients << pSocket;
    conexionWS.websocket = pSocket;

}


void TestServer::processTextMessage(QString message)
{
    QString respuesta;
    pClient = qobject_cast<QWebSocket*>(sender());
    qDebug() << "De:" << pClient << "Mensaje recibido:" << message;

    QString xmlToValidate = xmlManager.xmlMessage(message);

    if (xmlToValidate == "order")
    {
        QString xmlName = xmlManager.makeFiles("order", message);
        if (xmlManager.validatexml(xmlName, "newOrder.xsd"))
        {
            newOrder();
        }
    }
    else if (xmlToValidate == "find")
    {
        QString xmlName = xmlManager.makeFiles("find", message);
        qDebug() << xmlName;
        if (xmlManager.validatexml(xmlName, "findOrder.xsd"))
        {
            respuesta = xmlManager.writeOrderStatusXml(findOrder());

            pClient->sendTextMessage("9findOrder" + respuesta);
        }
    }
    else if (xmlToValidate == "login")
    {
        QString xmlName = xmlManager.makeFiles("login", message);
        if (xmlManager.validatexml(xmlName, "Login.xsd"))
        {
            respuesta = checkLogin();
            pClient->sendTextMessage("5login" + respuesta);
        }
    }
    else if (xmlToValidate == "orders")
    {

        QString xmlName = xmlManager.makeFiles("orders", message);
        if (xmlManager.validatexml(xmlName, "Orders.xsd"))
        {
            pClient->sendTextMessage(xmlManager.writeOrdersXml(dbManager.availableOrders()));
        }
    }
    else if (xmlToValidate == "newOrderStatus")
    {
        QString xmlName = xmlManager.makeFiles("newOrderStatus", message);
        if (xmlManager.validatexml(xmlName, "newOrderStatus.xsd"))
        {
            auto tuple = xmlManager.readNewOrderStatus(message);
            dbManager.newOrderStatus(std::get<0>(tuple), std::get<1>(tuple));
        }
    }
}

void TestServer::newOrder()
{
    auto newOrderXml = xmlManager.readNewOrder();
    dbManager.newOrder(
        std::get<0>(newOrderXml), std::get<1>(newOrderXml), std::get<2>(newOrderXml));
}


QString TestServer::findOrder()
{
    qDebug() << conexionWS.user;
    QString idorder = xmlManager.readFindOrder();
    QString result{ "" };
    result = dbManager.findOrder(idorder);
    return result;
}

QString TestServer::checkLogin()
{

    auto LoginXML = xmlManager.readLogin();
    QString result = dbManager.checkLogin(std::get<0>(LoginXML), std::get<1>(LoginXML));
    return result;
}

void TestServer::checkProcessOrders()
{
    dbManager.checkProcessOrders();
}



void TestServer::socketDisconnected()
{
    QWebSocket* pClient = qobject_cast<QWebSocket*>(sender());
    qDebug() << "Socket desconectado:" << pClient;

    if (pClient)
    {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    } // end if
}
