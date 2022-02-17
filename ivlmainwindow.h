#ifndef IVLMAINWINDOW_H
#define IVLMAINWINDOW_H

#include <QMainWindow>

#include "qcustomplot.h"
#include "ui_mainwindow.h"
#include "ivlserialport.h"
#include "ivlserver.h"


namespace Ui {
class IVLMainWindow;
}



class IVLMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit IVLMainWindow(QWidget *parent = 0);
    ~IVLMainWindow();

private:
    Ui::MainWindow *m_ui;
    IVLSerialPort  *m_ivlSerial;
    IVLServer      *m_ivlTCPServer;
    QTimer         *m_dataTimer;

    int pointPosition;
    QMap<QString, QVector<double>> dataToGrafs;

private:

    void setupAxisRect(QCustomPlot* customPlot , QVector<QCPRange>& range);

private slots:
    void slotRepaint();
};

#endif // IVLMAINWINDOW_H
