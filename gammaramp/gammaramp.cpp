// Qt:
#include <QString>
#include <QDebug>

// Application:
#include "gammaramp.h"

// Constructor:
GammaRamp::GammaRamp()
{
    // Initialize all variables:
#ifdef Q_OS_WIN
    hGDI32 = NULL;
    hScreenDC = NULL;
    pGetDeviceGammaRamp = NULL;
    pSetDeviceGammaRamp = NULL;
#elif defined(Q_OS_OSX)
#endif
}

// Destructor:
GammaRamp::~GammaRamp()
{
#ifdef Q_OS_WIN
    freeLibrary();
#elif defined(Q_OS_OSX)
#endif
}

#ifdef Q_OS_WIN

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
bool GammaRamp::setColorPalette(HDC hDC, const std::vector<int> &vRed, const std::vector<int> &vGreen, const std::vector<int> &vBlue)
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

    return (bool)bReturn;
}
#elif defined(Q_OS_OSX)
// Set color palette:
bool GammaRamp::setColorPalette(const std::vector<int> &vRed, const std::vector<int> &vGreen, const std::vector<int> &vBlue)
{
    CGDirectDisplayID displayId = CGMainDisplayID();
    uint32_t displayCount = 0;
    uint32_t displayMaxCount = 16;
    CGDirectDisplayID displayList[16];

    CGError error = CGGetActiveDisplayList(displayMaxCount, displayList, &displayCount);

    qDebug() << "setColorPalete" << displayId << displayCount << (int)error << vRed.size();

    if (error != kCGErrorSuccess) {
        qDebug() << "No Active Display";
        return false;
    }
    else if (displayCount > 0) {
        // gamma
        uint32_t capacity = vRed.size();
        uint32_t sampleCount = capacity;
        CGGammaValue *redTable = new CGGammaValue[capacity];
        CGGammaValue *greenTable = new CGGammaValue[capacity];
        CGGammaValue *blueTable = new CGGammaValue[capacity];

        for (int i = 0; i < vRed.size(); i++) {
            redTable[i] = float(vRed[i]) / 256.0;
            greenTable[i] = float(vGreen[i]) / 256.0;
            blueTable[i] = float(vBlue[i]) / 256.0;
            //qDebug() << "table value" << redTable[i] << greenTable[i] << blueTable[i];
        }

        for (int i = 0; i < displayCount; i++) {
            displayId = displayList[i];

            error = CGSetDisplayTransferByTable(displayId, sampleCount, redTable, greenTable, blueTable);
            //qDebug() << "set table" << displayId << sampleCount << (int) error;
        }
    }

    return true;
}
#endif

// Set blue light parameters:
bool GammaRamp::createColorPalette(const QColor &startColor, const QColor &stopColor)
{
    qDebug() << "createColorPalette" << startColor << stopColor;
    int iDeltaRed = abs(stopColor.red()-startColor.red());
    int iDeltaGreen = abs(stopColor.green()-startColor.green());
    int iDeltaBlue = abs(stopColor.blue()-startColor.blue());

    double iRedStep = (double)iDeltaRed/(double)255;
    double iGreenStep = (double)iDeltaGreen/(double)255;
    double iBlueStep = (double)iDeltaBlue/(double)255;

    std::vector<int> vRed, vGreen, vBlue;
    for (int i=0; i<256; i++)
    {
        int iRedValue = qRound(qMin(startColor.red(), stopColor.red()) + i*iRedStep);
        if (iRedValue > 255)
            iRedValue = 255;
        vRed.push_back(iRedValue);
        int iGreenValue = qRound(qMin(startColor.green(), stopColor.green()) + i*iGreenStep);
        if (iGreenValue > 255)
            iGreenValue = 255;
        vGreen.push_back(iGreenValue);
        int iBlueValue = qRound(qMin(startColor.blue(), stopColor.blue()) + i*iBlueStep);
        if (iBlueValue > 255)
            iBlueValue = 255;
        vBlue.push_back(iBlueValue);
        //qDebug() << iRedValue << iGreenValue << iBlueValue;
    }

    // Apply color palette:
#ifdef Q_OS_WIN
    return setColorPalette(NULL, vRed, vGreen, vBlue);
#elif defined(Q_OS_OSX)
    return setColorPalette(vRed, vGreen, vBlue);
#endif
}
