#ifndef IVLMAINWINDOW_H
#define IVLMAINWINDOW_H

#include <QMainWindow>

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
    Ui::IVLMainWindow *ui;
};

#endif // IVLMAINWINDOW_H
