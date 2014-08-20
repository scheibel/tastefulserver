#include <QCoreApplication>
#include <QString>

#include "Server.h"

int main(int argc, char ** argv)
{
    QCoreApplication app(argc, argv);
    Server server;

    server.listen(QHostAddress::Any, 8080);

    return app.exec();
}
