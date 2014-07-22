#include <QtWidgets>
#include "interface.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    interface1 w;
    w.show();
    
    return a.exec();
}
