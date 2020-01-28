#include "authorizationform.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AuthorizationForm w;
    w.show();
    return a.exec();
}
