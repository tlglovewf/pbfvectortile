#include "pbfshow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PbfShow w;
    w.show();

    return a.exec();
}
