#ifndef GAMMARAMP_H_
#define GAMMARAMP_H_

// Qt:
#include <Windows.h>
#include <QVector>

// Application:
#include "gammaramp_global.h"

class GAMMARAMPSHARED_EXPORT GammaRamp
{
public:
    // Constructor:
    GammaRamp();

    // Destructor:
    ~GammaRamp();

    // Set blue light parameters:
    void setBlueLightReducerParameters(WORD iMinRed, WORD iMaxRed, WORD iMinGreen, WORD iMaxGreen, WORD iMinBlue, WORD iMaxBlue);

private:
    // Load library:
    BOOL loadLibrary();

    // Free library:
    void freeLibrary();

    // Load library if needed:
    BOOL loadLibraryIfNeeded();

    // Set device gamma ramp:
    BOOL setDeviceGammaRamp(HDC hDC, LPVOID lpRamp);

    // Get device gamma ramp:
    BOOL getDeviceGammaRamp(HDC hDC, LPVOID lpRamp);

    // Set color palette:
    BOOL setColorPalette(HDC, const std::vector<WORD> &vRed, const std::vector<WORD> &vGreen, const std::vector<WORD> &vBlue);

protected:
    HMODULE hGDI32;
    HDC hScreenDC;
    typedef BOOL (WINAPI *Type_SetDeviceGammaRamp)(HDC hDC, LPVOID lpRamp);
    Type_SetDeviceGammaRamp pGetDeviceGammaRamp;
    Type_SetDeviceGammaRamp pSetDeviceGammaRamp;
};
#endif
