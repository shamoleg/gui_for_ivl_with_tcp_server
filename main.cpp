#include "ivlmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IVLMainWindow w;
    w.show();

    return a.exec();
}
