// Qt:
#include <QDebug>

// Application:
#include "blynk.h"
#include "controller.h"

// Blynk singleton:
Blynk *Blynk::sBlynk = 0;

// Constructor:
Blynk::Blynk(QObject *parent) : m_pController(NULL)
{
    Q_UNUSED(parent);
    m_pController = new Controller(this);
}

// Return kemanage singleton:
Blynk *Blynk::instance()
{
    if (!sBlynk)
        sBlynk = new Blynk();

    return sBlynk;
}

// Return controller:
Controller *Blynk::controller() const
{
    return m_pController;
}

// Startup:
bool Blynk::startup()
{
    // Start controller:
    if (!m_pController->startup())
        return false;

    return true;
}

// Shutdown:
void Blynk::shutdown()
{
    // Shutdown controller:
    m_pController->shutdown();
}
