#ifndef DIAESERVER_H
#define DIAESERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class DiaeServer : public QObject
{
    Q_OBJECT
public:
    explicit DiaeServer(quint16 port, QObject *parent = 0);
    virtual ~DiaeServer();

private Q_SLOTS:
    void onNewConnection();
    void processMessage(QString message);
    void socketDisconnected();

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;

};

#endif // DIAESERVER_H
