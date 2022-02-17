#ifndef IVLSERVER_H
#define IVLSERVER_H

#include <QWidget>
#include <QtNetwork>
#include <QTcpSocket>
#include <QMessageBox>
#include "ivlserialport.h"

class IVLServer : public QWidget
{
    Q_OBJECT
private:
    IVLSerialPort *m_ivlSerial;
    QTcpServer    *m_ptcpServer;
    quint16        m_nNextBlockSize;

    void sendToClient(QTcpSocket* pSocket, const QString& str);

public:
    IVLServer(int nPort, IVLSerialPort* serial, QWidget* parent=0);

public slots:
    virtual void slotNewConnection();
    void slotReadClient();
};

#endif // IVLSERVER_H
