#include "socketdemo.h"
#define YB_HOST "ws://10.21.3.62:4001/socket.io/1/websocket/"//"localhost"
#define YB_PORT 4001

//http://10.21.3.62:4008/getNode?userid=123456
SocketDemo::SocketDemo(QObject *parent) :
    QObject(parent)
{
    m_pTcpClient = new QTcpSocket(this);
    connect(m_pTcpClient,SIGNAL(connected()),this,SLOT(slot_tcp_connected()));
    connect(m_pTcpClient,SIGNAL(disconnected()),this,SLOT(slot_tcp_disconnected()));
    connect(m_pTcpClient,SIGNAL(readyRead()),this,SLOT(slot_tcp_readyRead()));
    connect(m_pTcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slot_tcp_error(QAbstractSocket::SocketError)));
}

SocketDemo::~SocketDemo()
{
    m_pTcpClient->deleteLater();
}

void SocketDemo::connectPhone(const QString& tokenString)
{
    qDebug()<<"tokenString "<<tokenString;
    QStringList lst = tokenString.split(":");
    QString tokenUrl;
    if(lst.count()>0){
        tokenUrl = lst.at(0);
    }
    QString hostUrl;
    hostUrl.clear();
    hostUrl.append(YB_HOST);
    hostUrl.append(tokenUrl);
    qDebug()<<"hostUrl "<<hostUrl;
    m_pTcpClient->connectToHost(hostUrl,YB_PORT);
}

void SocketDemo::buildJSon(const QString &jsonString)
{

}

void SocketDemo::processData()
{
    //    CParseAppInstallDataJson parseJson;
    //    parseJson.parseJson(m_recv);

    //    //通知APP管理模块删除命令
    //    CAppDataManager::getInstance()->delTask(m_recv.m_cmd);
    //    CAppDataManager::getInstance()->notifyUI(m_recv.m_cmd,0);
}

void SocketDemo::slot_tcp_connected()
{
    qDebug()<<"SocketDemo::slot_tcp_connected():connected";
    QString m_req = "hello nodejs";
    buildJSon(m_req);
    //qDebug()<<m_req.m_strParam;
    int result = m_pTcpClient->write(m_req.toUtf8().data(),m_req.toUtf8().length());
    //发送数据,若发送数据失败，则关闭连接重新连接
    if (result == -1)
    {
        qDebug()<<"SocketDemo::slot_tcp_connected(): send cmd error.[cmdJson]:"<<m_req;
        m_pTcpClient->abort();//关闭连接
    }
}

void SocketDemo::slot_tcp_readyRead()
{
    QString m_recv;
    m_recv += m_pTcpClient->readAll();
    //qDebug()<<"SocketDemo::slot_tcp_readyRead()"<<m_recv.m_strResult;
    if(m_recv.contains("{\"state\":\"error\"}$") ||
            m_recv.contains("{\"error\":\"Exception:"))//连接出错
    {
        qDebug()<<"SocketDemo::slot_tcp_readyRead(): receive data error.";
        m_pTcpClient->abort();//关闭连接
    }
    else if(m_recv.contains("]$"))
    {
        //processData();
        m_pTcpClient->abort();//关闭连接
    }
}

void SocketDemo::slot_tcp_disconnected()
{
    //    qDebug()<<"SocketDemo::slot_tcp_disconnected():tcp disconnected.";
}

void SocketDemo::slot_tcp_error(QAbstractSocket::SocketError error)
{
    qDebug()<<"SocketDemo::slot_tcp_error():tcp disconnected error."<<error;
}
