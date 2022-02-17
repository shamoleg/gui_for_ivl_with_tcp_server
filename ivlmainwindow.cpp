#include "ivlmainwindow.h"


IVLMainWindow::IVLMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    m_IVLSerial = new IVLSerialPort;
    m_IVLSerial->connectToFirstFoundCOM();

    QVector<QCPRange> rangeFlow = {{-0.5, 1.5}, {1, 500}};
    this->setupAxisRect(m_ui->graphFlow, rangeFlow);

    QVector<QCPRange> rangeVolume = {{-1, 5}, {1, 500}};
    this->setupAxisRect(m_ui->graphVolume, rangeVolume);

    QPalette p(this->palette());
    p.setColor(QPalette::Window, Qt::white);
    m_ui->tabSettings->setAutoFillBackground(true);
    m_ui->tabSettings->setPalette(p);

    QPixmap pix(":/img/img/logo.png");
    int w = m_ui->logo->width();
    int h = m_ui->logo->height();
    m_ui->logo->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));

}

IVLMainWindow::~IVLMainWindow()
{
    delete m_ui;
}

void IVLMainWindow::setupAxisRect(QCustomPlot* customPlot , QVector<QCPRange>& range)
{
    customPlot->plotLayout()->clear();

    QCPAxisRect* axisRect = new QCPAxisRect(customPlot, false);

    axisRect->addAxes(QCPAxis::atBottom | QCPAxis::atLeft);

    axisRect->axis(QCPAxis::atLeft)->setRange(range[0]);
    axisRect->axis(QCPAxis::atLeft)->grid()->setVisible(true);
    axisRect->axis(QCPAxis::atBottom)->setRange(range[1]);
    axisRect->axis(QCPAxis::atBottom)->grid()->setVisible(true);

    customPlot->plotLayout()->addElement(0, 0,  axisRect);
    customPlot->addGraph(axisRect->axis(QCPAxis::atBottom),  axisRect->axis(QCPAxis::atLeft));
    customPlot->graph(0)->setPen(QPen(QColor(66, 145, 255), 4));
}

IVLSerialPort::IVLSerialPort(QWidget *parent):
    QWidget(parent)
{
    m_serial  = new QSerialPort;
    connect(m_serial, SIGNAL(readyRead()), this, SLOT(slotSerialRead()));
}

IVLSerialPort::~IVLSerialPort()
{
    delete m_serial;
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
        m_serial->setBaudRate(QSerialPort::Baud115200);
        m_serial->setStopBits(QSerialPort::OneStop);
        m_serial->setParity(QSerialPort::NoParity);
        m_serial->setFlowControl(QSerialPort::NoFlowControl);
    }
    if (!m_serial->open(QIODevice::ReadWrite))
    {
        qWarning()<< "[WARN]: Failed to connect COM port";
    }else
    {

    }
}

void IVLSerialPort::slotSerialRead()
{
    m_message = m_serial->read(16);

    emit signalDataRead();
}

