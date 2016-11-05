// Qt:
#include <QApplication>
#include <QDebug>

// Application:
#include "blynk.h"
#include <qtsingleapplication.h>

int main(int argc, char *argv[])
{
    QtSingleApplication app("BlynkInstance", argc, argv);

    // Launch application:
    Blynk *pBlynk = Blynk::instance();
    if (!pBlynk)
        return 0;

     // Start application:
    if (pBlynk->startup())
    {
        // Run:
        app.exec();
    }

    qDebug() << "EXIT";

    // Shutdown:
    pBlynk->shutdown();

    // Delete application:
    delete pBlynk;

    return 1;
}
