// Application:
#include "windowsmonitor.h"

// Qt:
#include <QDebug>

// Constructor:
WindowsMonitor::WindowsMonitor() : m_pSavedRamps(NULL),
    hGDI32(NULL)
{

}

// Destructor:
WindowsMonitor::~WindowsMonitor()
{
    if (m_pSavedRamps)
    {
        free(m_pSavedRamps);
        m_pSavedRamps = NULL;
    }
}

// Load library:
bool WindowsMonitor::loadLibrary()
{
    bool bReturn = false;

    // Free current:
    freeLibrary();

    // Load the GDI library:
    LPCWSTR a = L"gdi32.dll";
    hGDI32 = ::LoadLibrary(a);
    if (hGDI32 != NULL)
    {
        // Get the addresses of GetDeviceGammaRamp:
        pGetDeviceGammaRamp =
                (Type_SetDeviceGammaRamp)GetProcAddress(hGDI32, "GetDeviceGammaRamp");

        // Get the addresses of SetDeviceGammaRamp:
        pSetDeviceGammaRamp =
                (Type_SetDeviceGammaRamp)GetProcAddress(hGDI32, "SetDeviceGammaRamp");

        // Get the addresses of GetDeviceCaps;
        pGetDeviceCaps =
                (Type_GetDeviceCaps)GetProcAddress(hGDI32, "GetDeviceCaps");

        // Return true only if these functions exist:
        if (pGetDeviceGammaRamp == NULL || pSetDeviceGammaRamp == NULL || pGetDeviceCaps == NULL)
            freeLibrary();
        else
            bReturn = true;
    }

    return bReturn;
}

// Load library if needed:
bool WindowsMonitor::loadLibraryIfNeeded()
{
    if (hGDI32 == NULL)
        loadLibrary();

    return (pGetDeviceGammaRamp != NULL && pSetDeviceGammaRamp != NULL && pGetDeviceCaps != NULL);
}

// Free library:
void WindowsMonitor::freeLibrary()
{
    // Free the GDI library:
    if (hGDI32 != NULL)
    {
        ::FreeLibrary(hGDI32);
        hGDI32 = NULL;
    }
}

// Init:
bool WindowsMonitor::init()
{
    hGDI32 = NULL;
    m_pSavedRamps = NULL;
    return true;
}

// Start:
bool WindowsMonitor::start()
{
    if (loadLibraryIfNeeded())
    {
        // Open device context:
        HDC hDC = GetDC(NULL);
        if (hDC == NULL) {
            qDebug() << "Unable to open device context";
            return false;
        }

        // Check support for gamma ramps:
        int cmcap = pGetDeviceCaps(hDC, COLORMGMTCAPS);
        if (cmcap != CM_GAMMA_RAMP) {
            qDebug() << "Display device does not support gamma ramps";
            return false;
        }

        // Allocate space for saved gamma ramps:
        m_pSavedRamps = (WORD *)malloc(3*GAMMA_RAMP_SIZE*sizeof(WORD));
        if (m_pSavedRamps == NULL) {
            qDebug() << "Malloc error";
            ReleaseDC(NULL, hDC);
            return false;
        }

        // Save current gamma ramps so we can restore them at program exit:
        bool bResult = pGetDeviceGammaRamp(hDC, m_pSavedRamps);
        if (!bResult) {
            qDebug() << "Unable to save current gamma ramp";
            ReleaseDC(NULL, hDC);
            return false;
        }

        // Release device context:
        ReleaseDC(NULL, hDC);

        return true;
    }

    return false;
}

// Restore:
void WindowsMonitor::restore()
{
    if (loadLibraryIfNeeded())
    {
        // Open device context:
        HDC hDC = GetDC(NULL);
        if (hDC == NULL) {
            qDebug() << "Unable to open device context";
            return;
        }

        // Restore gamma ramps:
        bool r = pSetDeviceGammaRamp(hDC, m_pSavedRamps);
        if (!r)
            qDebug() << "Unable to restore gamma ramps";

        // Release device context:
        ReleaseDC(NULL, hDC);
    }
}

// Set temperature:
bool WindowsMonitor::setTemperature(int iTemperature, bool bForce)
{
    if (loadLibraryIfNeeded())
    {
        bool bExecute = bForce ? true : (m_iCurrentTemperature != iTemperature);
        if (bExecute)
        {
            // Restore:
            if (iTemperature <= 0)
                restore();
            else {
                // Open device context:
                HDC hDC = GetDC(NULL);
                if (hDC == NULL) {
                    qDebug() << "Unable to open device context";
                    return false;
                }

                // Create new gamma ramps:
                WORD *gamma_ramps = (WORD *)malloc(3*GAMMA_RAMP_SIZE*sizeof(WORD));
                if (gamma_ramps == NULL) {
                    qDebug() << "Malloc error";
                    ReleaseDC(NULL, hDC);
                    return false;
                }

                WORD *gamma_r = &gamma_ramps[0*GAMMA_RAMP_SIZE];
                WORD *gamma_g = &gamma_ramps[1*GAMMA_RAMP_SIZE];
                WORD *gamma_b = &gamma_ramps[2*GAMMA_RAMP_SIZE];

                // Initialize gamma ramps to pure state:
                for (int i = 0; i < GAMMA_RAMP_SIZE; i++) {
                    WORD value = (double)i/GAMMA_RAMP_SIZE *
                            (UINT16_MAX+1);
                    gamma_r[i] = value;
                    gamma_g[i] = value;
                    gamma_b[i] = value;
                }

                // Color ramp fill:
                colorRampFill(gamma_r, gamma_g, gamma_b, GAMMA_RAMP_SIZE, iTemperature);

                // Set new gamma ramps:
                bool r = pSetDeviceGammaRamp(hDC, gamma_ramps);
                if (!r) {
                    qDebug() << "Unable to set gamma ramps.\n";
                    free(gamma_ramps);
                    ReleaseDC(NULL, hDC);
                    return false;
                }

                free(gamma_ramps);

                // Release device context:
                ReleaseDC(NULL, hDC);
            }

            // Set current temperature:
            m_iCurrentTemperature = iTemperature;

            return true;
        }
    }

    return false;
}

// Color ramp fill:
void WindowsMonitor::colorRampFill(uint16_t *gamma_r, uint16_t *gamma_g, uint16_t *gamma_b,
                                   int iSize, int iTemperature)
{
    // Approximate white point:
    float white_point[3];
    float alpha = (iTemperature % 100) / 100.0;
    int temp_index = ((iTemperature - 1000) / 100)*3;
    interpolateColor(alpha, &blackbody_color[temp_index], &blackbody_color[temp_index+3], white_point);

    for (int i = 0; i < iSize; i++) {
        gamma_r[i] = fillFunction(white_point, (double)gamma_r[i]/(UINT16_MAX+1), 0) *
                (UINT16_MAX+1);
        gamma_g[i] = fillFunction(white_point, (double)gamma_g[i]/(UINT16_MAX+1), 1) *
                (UINT16_MAX+1);
        gamma_b[i] = fillFunction(white_point, (double)gamma_b[i]/(UINT16_MAX+1), 2) *
                (UINT16_MAX+1);
    }
}
