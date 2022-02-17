#include "ivlmainwindow.h"
#include "ui_ivlmainwindow.h"

IVLMainWindow::IVLMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IVLMainWindow)
{
    ui->setupUi(this);
}

IVLMainWindow::~IVLMainWindow()
{
    delete ui;
}
