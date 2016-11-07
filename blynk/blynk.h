#ifndef BLYNK_H
#define BLYNK_H

// Qt:
#include <QObject>

// Application:
#include "iservice.h"

class Controller;

// Main application:
class Blynk : public QObject, public IService
{
    Q_OBJECT

public:
    // Return an instance of Blynk:
    static Blynk *instance();

    // Startup:
    virtual bool startup();

    // Shutdown:
    virtual void shutdown();

    // Destructor:
    virtual ~Blynk() {

    }

    // Return controller:
    Controller *controller() const;

private:
    // Constructor:
    Blynk(QObject *parent=0);

private:
    // Farmers Fridge singleton:
    static Blynk *sBlynk;

    // Controller singleton:
    Controller *m_pController;
};

#endif // BLYNK_H
