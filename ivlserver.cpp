#include "ivlserver.h"

IVLServer::IVLServer(int nPort, IVLSerialPort* serial, QWidget *parent) :
    QWidget(parent), m_nNextBlockSize(0)
{
    m_ptcpServer = new QTcpServer(this);
    m_ivlSerial = serial;


    if (!m_ptcpServer->listen(QHostAddress::Any, nPort)) {
        QMessageBox::critical(0,
                              "Server Error",
                              "Unable to start the server:" +
                              m_ptcpServer->errorString());
        m_ptcpServer->close();
        return;
    }

    connect(m_ptcpServer, SIGNAL(newConnection()),
            this,         SLOT(slotNewConnection()));

}

void IVLServer::slotNewConnection()
{
    QTcpSocket* pClientSoket = m_ptcpServer->nextPendingConnection();

    connect(pClientSoket, SIGNAL(disconnected()),
            pClientSoket, SLOT(deleteLater()));

    connect(pClientSoket, SIGNAL(readyRead()),
            this        , SLOT(slotReadClient()));

    sendToClient(pClientSoket, "Server Response: Connected!");
}

void IVLServer::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QByteArray msgFromClient;

    msgFromClient.append(pClientSocket->readAll());
    qDebug() << QString(msgFromClient);

    auto data = m_ivlSerial->m_ivlMessage->getData();

    QString msgToClient = "MES: " + QString::number(data[0]);
    sendToClient(pClientSocket, msgToClient);
}

void IVLServer::sendToClient(QTcpSocket* pSoket, const QString& str)
{
    pSoket->write(str.toLocal8Bit());
}
