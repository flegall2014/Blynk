#ifndef WINDOWSMONITOR_H
#define WINDOWSMONITOR_H

// Application:
#include "monitor.h"
#include <stdint.h>
#include <windows.h>
#include "gammaramp_global.h"
#define UINT16MAX 65535

class GAMMARAMPSHARED_EXPORT WindowsMonitor : public Monitor
{
public:
    // Constructor:
    WindowsMonitor();

    // Destructor:
    virtual ~WindowsMonitor();

    // Init:
    virtual bool init();

    // Start:
    virtual bool start();

    // Restore:
    virtual void restore();

    // Set temperature:
    virtual bool setTemperature(int iTemperature, bool bForce=false);

protected:
    HMODULE hGDI32;
    typedef bool (WINAPI *Type_SetDeviceGammaRamp)(HDC hDC, LPVOID lpRamp);
    typedef int (WINAPI *Type_GetDeviceCaps)(HDC hdc,int index);
    Type_SetDeviceGammaRamp pGetDeviceGammaRamp;
    Type_SetDeviceGammaRamp pSetDeviceGammaRamp;
    Type_GetDeviceCaps pGetDeviceCaps;

    // Saved ramps:
    unsigned short *m_pSavedRamps;

private:
    // Load library:
    bool loadLibrary();

    // Load library if needed:
    bool loadLibraryIfNeeded();

    // Free library:
    void freeLibrary();

    // Color ramp fill:
    void colorRampFill(uint16_t *gamma_r, uint16_t *gamma_g, uint16_t *gamma_b,
               int iSize, int iTemperature);
};

#endif // WINDOWSMONITOR_H
