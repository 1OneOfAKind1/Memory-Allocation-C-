#include "memoryallocation.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Memoryallocation w;
    w.show();

    return a.exec();
}
