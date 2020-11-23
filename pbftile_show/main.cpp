#include "pbfshow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(argc == 2)
    {
       static PbfShow w(argv[1]);
       w.show();
    }
    else
    {
       static PbfShow w;
       w.show();
    }



    return a.exec();
}
