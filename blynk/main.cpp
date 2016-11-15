// Qt:
#include <QApplication>
#include <QFile>
#include <QDebug>

// Application:
#include "blynk.h"
#include <qtsingleapplication.h>

// Set style sheet:
void setStyleSheet()
{
    QFile file(":/css/style.css");
    if (file.open(QFile::ReadOnly))
    {
        QString sStyleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(sStyleSheet);
        file.close();
    }
}

int main(int argc, char *argv[])
{
    QtSingleApplication app("BlynkInstance", argc, argv);
    setStyleSheet();

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
