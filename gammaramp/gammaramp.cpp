// Qt:
#include <QString>
#include <QDebug>

// Application:
#include "gammaramp.h"

// Constructor:
GammaRamp::GammaRamp()
{
    // Initialize all variables:
    hGDI32 = NULL;
    hScreenDC = NULL;
    pGetDeviceGammaRamp = NULL;
    pSetDeviceGammaRamp = NULL;
}

// Destructor:
GammaRamp::~GammaRamp()
{
    freeLibrary();
}

// Load library:
BOOL GammaRamp::loadLibrary()
{
    BOOL bReturn = FALSE;

    // Free current:
    freeLibrary();

    // Load the GDI library:
    LPCWSTR a = L"gdi32.dll";
    hGDI32 = ::LoadLibrary(a);
    if (hGDI32 != NULL)
    {
        // Get the addresses of GetDeviceGammaRamp and SetDeviceGammaRamp API functions:
        pGetDeviceGammaRamp =
                (Type_SetDeviceGammaRamp)GetProcAddress(hGDI32, "GetDeviceGammaRamp");

        pSetDeviceGammaRamp =
                (Type_SetDeviceGammaRamp)GetProcAddress(hGDI32, "SetDeviceGammaRamp");

        // Return TRUE only if these functions exist:
        if (pGetDeviceGammaRamp == NULL || pSetDeviceGammaRamp == NULL)
            freeLibrary();
        else
            bReturn = TRUE;
    }

    return bReturn;
}

// Free library:
void GammaRamp::freeLibrary()
{
    // Free the GDI library:
    if (hGDI32 != NULL)
    {
        ::FreeLibrary(hGDI32);
        hGDI32 = NULL;
    }
}

// Load library if needed:
BOOL GammaRamp::loadLibraryIfNeeded()
{
    BOOL bReturn = FALSE;

    if (hGDI32 == NULL)
        loadLibrary();

    if (pGetDeviceGammaRamp != NULL && pSetDeviceGammaRamp != NULL)
        bReturn = TRUE;

    return bReturn;
}

// Set device gamma ramp:
BOOL GammaRamp::setDeviceGammaRamp(HDC hDC, LPVOID lpRamp)
{
    // Call to SetDeviceGammaRamp only if this function is successfully loaded:
    if (loadLibraryIfNeeded())
    {
        BOOL bResult = pSetDeviceGammaRamp(hDC, lpRamp);
        qDebug() << "*** RESULT OF APPLYING GAMMA RAMP ***" << bResult;
        return bResult;
    }
    qDebug() << "*** LOADLIBRARYIFNEEDED FAILED ***";
    return FALSE;
}

// Get device gamma ramp:
BOOL GammaRamp::getDeviceGammaRamp(HDC hDC, LPVOID lpRamp)
{
    // Call to GetDeviceGammaRamp only if this function is successfully loaded:
    if (loadLibraryIfNeeded())
    {
        return pGetDeviceGammaRamp(hDC, lpRamp);
    }
    return FALSE;
}

// Set color palette:
BOOL GammaRamp::setColorPalette(HDC hDC, const std::vector<int> &vRed, const std::vector<int> &vGreen, const std::vector<int> &vBlue)
{
    BOOL bReturn = FALSE;
    HDC hGammaDC = hDC;

    //Load the display device context of the entire screen if hDC is NULL.
    if (hDC == NULL)
        hGammaDC = GetDC(NULL);

    if (hGammaDC != NULL)
    {
        // Generate color table:
        WORD GammaArray[3][256];

        for (int iIndex = 0; iIndex < 256; iIndex++)
        {
            GammaArray[0][iIndex] = (WORD)vRed[iIndex]*257;
            GammaArray[1][iIndex] = (WORD)vGreen[iIndex]*257;
            GammaArray[2][iIndex] = (WORD)vBlue[iIndex]*257;
        }

        // Set the GammaArray values into the display device context:
        bReturn = setDeviceGammaRamp(hGammaDC, GammaArray);
    }

    if (hDC == NULL)
        ReleaseDC(NULL, hGammaDC);

    return bReturn;
}

// Set blue light parameters:
void GammaRamp::setBlueLightReducerParameters(int iMinRed, int iMaxRed, int iMinGreen, int iMaxGreen, int iMinBlue, int iMaxBlue)
{
    int iDeltaRed = abs(iMaxRed-iMinRed);
    int iDeltaGreen = abs(iMaxGreen-iMinGreen);
    int iDeltaBlue = abs(iMaxBlue-iMinBlue);

    double iRedStep = (double)iDeltaRed/(double)256;
    double iGreenStep = (double)iDeltaGreen/(double)256;
    double iBlueStep = (double)iDeltaBlue/(double)256;

    std::vector<int> vRed, vGreen, vBlue;
    for (int i=0; i<256; i++)
    {
        vRed.push_back(qRound(qMin(iMinRed, iMaxRed) + i*iRedStep));
        vGreen.push_back(qRound(qMin(iMinGreen, iMaxGreen) + i*iGreenStep));
        vBlue.push_back(qRound(qMin(iMinBlue, iMaxBlue) + i*iBlueStep));
    }

    // Apply color palette:
    setColorPalette(NULL, vRed, vGreen, vBlue);
}
