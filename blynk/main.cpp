// Qt:
#include <QApplication>
#include <QFile>
#include <QFont>
#include <QFontDatabase>
#include <QDebug>

// Application:
#include "blynk.h"
#include <qtsingleapplication.h>

// Set style sheet:
void setStyleSheet()
{
#ifdef Q_OS_WIN
    QFile file(":/css/style_windows.css");
#elif defined(Q_OS_OSX)
    QFile file(":/css/style_macos.css");
#endif
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
