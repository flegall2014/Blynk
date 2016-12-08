#ifndef IMONITOR_H
#define IMONITOR_H
#include <QTime>

class IMonitor
{
public:
    // Init:
    virtual bool init() = 0;

    // Start:
    virtual bool start() = 0;

    // Restore:
    virtual void restore() = 0;

    // Set temperature:
    virtual bool setTemperature(int iTemperature, bool bForce=false) = 0;

    // Destructor:
    virtual ~IMonitor() {

    }
};

#endif // ANIMATEDIMAGE_H
