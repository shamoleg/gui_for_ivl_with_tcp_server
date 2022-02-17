#include "ivlmainwindow.h"


IVLMainWindow::IVLMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

IVLMainWindow::~IVLMainWindow()
{
    delete ui;
}
