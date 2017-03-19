#include <QCoreApplication>
#include "diaeserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DiaeServer server(1234);

    return a.exec();
}
