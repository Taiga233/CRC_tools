
#include "crc.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    crc w;
    w.show();
    return a.exec();
}
