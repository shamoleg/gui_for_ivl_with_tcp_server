#ifndef IVLSERIALPORT_H
#define IVLSERIALPORT_H

#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>

enum DataPosition{
    VOLUME   = 1,
    PRESSURE = 2,
    OXYGEN   = 3,
    SETPOINT = 6,
};

struct IVLMessage {
public:
    IVLMessage();

    void setDataRecivedIVLSerial(QByteArray& msg);
    void setDataToSend(DataPosition position, int data);

    QVector<double> getData();

private:
    QByteArray dataRecived;
    QByteArray dataToSend;

    QVector<double> dataToPlot;

    uint8_t sizeOfData;
    uint8_t sizeOfOneNum;
    uint32_t scaleToDouble;
};



class IVLSerialPort: public QWidget{
    Q_OBJECT

public:
    explicit IVLSerialPort(QWidget* parent=0);
    ~IVLSerialPort();

    void connectToFirstFoundCOM() const;

    IVLMessage  *m_ivlMessage;

private:
    QSerialPort *m_serial; 
    QTimer      *m_reconnectTimer;

private slots:
    void slotSerialRead();
    void slotReconnect();

signals:
    void signalDataRead();
};

#endif // IVLSERIALPORT_H
