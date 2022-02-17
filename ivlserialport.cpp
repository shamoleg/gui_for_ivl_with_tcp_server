#include "ivlserialport.h"
#include <iostream>
#include <exception>

IVLSerialPort::IVLSerialPort(QWidget *parent):
    QWidget(parent)
{
    m_serial  = new QSerialPort;
    m_ivlMessage = new IVLMessage;

    connect(m_serial, SIGNAL(readyRead()), this, SLOT(slotSerialRead()));
    connect(m_serial, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(connectionCheckSlot(QSerialPort::SerialPortError)));

    m_reconnectTimer = new QTimer;
    connect(m_reconnectTimer, SIGNAL(timeout()), this, SLOT(slotReconnect()));
}

IVLSerialPort::~IVLSerialPort()
{
    delete m_serial;
    delete m_ivlMessage;
}

void IVLSerialPort::connectToFirstFoundCOM() const{
    m_serial->close();
    const auto listPort = QSerialPortInfo::availablePorts();
    qDebug() << "[LOG ]: Number of connection is " << listPort.size();

    if(listPort.size()!= 0)
    {
        qDebug()<< "[LOG ]: Connect to " <<  listPort[0].serialNumber() << listPort[0].portName();
        m_serial->setPortName(listPort[0].portName());
        m_serial->setDataBits(QSerialPort::Data8);
        m_serial->setBaudRate(QSerialPort::Baud57600);
        m_serial->setStopBits(QSerialPort::OneStop);
        m_serial->setParity(QSerialPort::NoParity);
        m_serial->setFlowControl(QSerialPort::NoFlowControl);
    }
    if (!m_serial->open(QIODevice::ReadWrite))
    {
        m_reconnectTimer->start(15000);
        qWarning()<< "[WARN]: Failed to connect COM port";
    }else
    {
        m_reconnectTimer->stop();
        qDebug() << "[LOG ]: UART is connected";
    }
}

void IVLSerialPort::slotSerialRead()
{
    QByteArray msg = m_serial->readAll();
    m_ivlMessage->setDataRecivedIVLSerial(msg);
    emit signalDataRead();
}

void IVLSerialPort::slotReconnect()
{
    m_reconnectTimer->stop();
    this->connectToFirstFoundCOM();
}


IVLMessage::IVLMessage()
{
    sizeOfData = 8;
    sizeOfOneNum = 4;
    scaleToDouble = 1000000;

    dataToSend.resize(7);
    dataToSend[0] = 0x21;
    dataToSend[5] = 0x12;


    dataRecived.resize(sizeOfData);
}

void IVLMessage::setDataRecivedIVLSerial(QByteArray& msg)
{
    this->dataRecived = msg;
}

void IVLMessage::setDataToSend(DataPosition position, int data)
{
    this->dataToSend[position] = data;
}

QVector<double> IVLMessage::getData()
{

    QVector<double> msgResult(1);
    QByteArray msgRecived = dataRecived;

    //TEMP
    msgResult[0] = msgRecived.toHex().toInt();
//    uint8_t sizeOfOneNum = 4;
//    uint32_t scaleToDouble = 1000000;
//    for(uint8_t iter = 0; iter <= msgRecived.length() ; iter += sizeOfOneNum)
//    {
//        QByteArray oneNum = msgRecived.mid(iter, sizeOfOneNum);
//        std::reverse(oneNum.begin(), oneNum.end());
//        msgResult[iter/sizeOfOneNum] = (int32_t)oneNum.toHex().toUInt(0, 16);
//        if(iter/sizeOfOneNum != 2){
//            msgResult[iter/sizeOfOneNum] /= scaleToDouble;
//        }
//    }

    return msgResult;
}


