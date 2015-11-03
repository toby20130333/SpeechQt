#ifndef SOCKETDEMO_H
#define SOCKETDEMO_H

#include <QObject>
#include <QTcpSocket>
#include <QStringList>
class SocketDemo : public QObject
{
    Q_OBJECT
public:
    explicit SocketDemo(QObject *parent = 0);
    ~SocketDemo();
    void connectPhone(const QString& tokenString);
    void buildJSon(const QString& jsonString);
private:
    void processData();
signals:

public slots:
    void slot_tcp_connected();
    void slot_tcp_readyRead();
    void slot_tcp_disconnected();
    void slot_tcp_error(QAbstractSocket::SocketError);
private:
   QTcpSocket *m_pTcpClient;
};

#endif // SOCKETDEMO_H
