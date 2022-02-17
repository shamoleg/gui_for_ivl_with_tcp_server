#include "ivlmainwindow.h"


IVLMainWindow::IVLMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);



    m_ivlSerial = new IVLSerialPort;
    m_ivlSerial->connectToFirstFoundCOM();

    m_ivlTCPServer = new IVLServer(2323, m_ivlSerial);

    dataToGrafs = {
        {"VOLUME_X", QVector<double>(511)},
        {"VOLUME_Y", QVector<double>(511)},

        {"PRESSURE_X", QVector<double>(511)},
        {"PRESSURE_Y", QVector<double>(511)},
    };
    pointPosition =-1;
    qDebug() << dataToGrafs["VOLUME_X"].size();

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

    m_dataTimer = new QTimer;
    m_dataTimer->start(50);

    connect(m_dataTimer, SIGNAL(timeout()), this, SLOT(slotRepaint()));

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

void IVLMainWindow::slotRepaint()
{

    QString spo = "Содержание кислорода: %1 \%";

    auto dataFromMassage = m_ivlSerial->m_ivlMessage->getData();
    m_ui->label->setText(spo.arg(dataFromMassage[0]));
    m_ui->label_2->setText(spo.arg(dataFromMassage[0]));

    const int sizeOfGap = 10;
    pointPosition++;

    dataToGrafs["VOLUME_X"][pointPosition] = dataFromMassage[0];
    dataToGrafs["VOLUME_Y"][pointPosition] = pointPosition;


    dataToGrafs["PRESSURE_X"][pointPosition] = dataFromMassage[0];
    dataToGrafs["PRESSURE_Y"][pointPosition] = pointPosition;

    for(int data = pointPosition+1; data < (pointPosition + sizeOfGap); data++){
        dataToGrafs["PRESSURE_X"][data] = qQNaN();
        dataToGrafs["VOLUME_X"][data] = qQNaN();
    }


    if (pointPosition > 500)
        pointPosition = 0;

    m_ui->graphFlow->graph(0)->setData(dataToGrafs["PRESSURE_Y"], dataToGrafs["PRESSURE_X"]);//0
    m_ui->graphFlow->replot();


    m_ui->graphVolume->graph(0)->setData(dataToGrafs["VOLUME_Y"], dataToGrafs["VOLUME_X"]);//1
    m_ui->graphVolume->replot();


}



