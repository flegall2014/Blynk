#ifndef ISERVICE_H
#define ISERVICE_H

class IService
{
public:
    // Startup:
    virtual bool startup() = 0;

    // Shutdown:
    virtual void shutdown() = 0;

    // Destructor:
    virtual ~IService() {

    }
};

#endif // ISERVICE_H
