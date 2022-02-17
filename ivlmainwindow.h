#ifndef IVLMAINWINDOW_H
#define IVLMAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "qcustomplot.h"
#include "ui_mainwindow.h"


namespace Ui {
class IVLMainWindow;
}

class IVLSerialPort: public QWidget{
    Q_OBJECT
private:
    QSerialPort *m_serial;
    QByteArray  m_message;

    ~IVLSerialPort();
public:
    explicit IVLSerialPort(QWidget* parent=0);
    void connectToFirstFoundCOM() const;

private slots:
    void slotSerialRead();

signals:
    void signalDataRead();
};

class IVLMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit IVLMainWindow(QWidget *parent = 0);
    ~IVLMainWindow();

private:
    Ui::MainWindow *m_ui;
    IVLSerialPort  *m_IVLSerial;

private:

    void setupAxisRect(QCustomPlot* customPlot , QVector<QCPRange>& range);
};

#endif // IVLMAINWINDOW_H
