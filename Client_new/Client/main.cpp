#include "signin.h"
#include <QApplication>
#include "chat.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SignIn w;
    w.show();
//    Home h("1454865804", "YDS");
//    h.show();
//      Chat c;
//      c.show();

    return a.exec();
}
