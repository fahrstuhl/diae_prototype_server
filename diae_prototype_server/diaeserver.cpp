#include "diaeserver.h"
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <QtCore/QDebug>

QT_USE_NAMESPACE

DiaeServer::DiaeServer(quint16 port, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(Q_NULLPTR)
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("DIAE Server"),
                                              QWebSocketServer::NonSecureMode,
                                              this);
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        qDebug() << "DIAE server on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &DiaeServer::onNewConnection);
    }
}

DiaeServer::~DiaeServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void DiaeServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    connect(pSocket, &QWebSocket::textMessageReceived, this, &DiaeServer::processMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &DiaeServer::socketDisconnected);
    m_clients << pSocket;
}

void DiaeServer::processMessage(QString message)
{
    QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());
    for (QWebSocket *pClient : qAsConst(m_clients)) {
        if (pClient != pSender)
        {
            pClient->sendTextMessage(message);
        }
    }
}

void DiaeServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
