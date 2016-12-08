// Application:
#include "macmonitor.h"
#include <CoreGraphics/CoreGraphics.h>

// Qt:
#include <QDebug>

// Constructor:
MacMonitor::MacMonitor() : m_pSavedRamps(NULL)
{

}

// Init:
bool MacMonitor::init()
{
    m_pSavedRamps = NULL;

    return true;
}

// Start:
bool MacMonitor::start()
{
    CGError error;
    uint32_t display_count;

    // Get display count:
    error = CGGetOnlineDisplayList(0, NULL, &display_count);
    if (error != kCGErrorSuccess)
        return false;

    CGDirectDisplayID display = CGMainDisplayID();
    uint32_t ramp_size = CGDisplayGammaTableCapacity(display);
    if (ramp_size == 0) {
        qDebug() << "Gamma ramp size too small: " << ramp_size;
        return false;
    }

    // Allocate space for saved ramps:
    m_pSavedRamps =
        (float *)malloc(3 * ramp_size * sizeof(float));
    if (m_pSavedRamps == NULL) {
        qDebug() << "Malloc error";
        return false;
    }

    float *gamma_r = &m_pSavedRamps[0*ramp_size];
    float *gamma_g = &m_pSavedRamps[1*ramp_size];
    float *gamma_b = &m_pSavedRamps[2*ramp_size];

    // Copy the ramps to allocated space:
    uint32_t sample_count;
    error = CGGetDisplayTransferByTable(display, ramp_size,
                        gamma_r, gamma_g, gamma_b,
                        &sample_count);
    if (error != kCGErrorSuccess ||
        sample_count != ramp_size) {
        qDebug() << "Unable to save current gamma ramp.\n";
        return false;
    }

    return true;
}

// Restore:
void MacMonitor::restore()
{
    CGDisplayRestoreColorSyncSettings();
}

// Set temperature:
bool MacMonitor::setTemperature(int iTemperature, bool bForce)
{
    bool bExecute = bForce ? true : (m_iCurrentTemperature != iTemperature);
    if (bExecute)
    {
        // Restore:
        if (iTemperature <= 0)
            restore();
        else {
            CGDirectDisplayID display = CGMainDisplayID();
            uint32_t ramp_size = CGDisplayGammaTableCapacity(display);

            // Create new gamma ramps:
            float *gamma_ramps = (float *)malloc(3*ramp_size*sizeof(float));
            if (gamma_ramps == NULL) {
                qDebug() << "Malloc error";
                return false;
            }

            float *gamma_r = &gamma_ramps[0*ramp_size];
            float *gamma_g = &gamma_ramps[1*ramp_size];
            float *gamma_b = &gamma_ramps[2*ramp_size];

            // Initialize gamma ramps to pure state:
            for (uint32_t i = 0; i < ramp_size; i++) {
                float value = (double)i/ramp_size;
                gamma_r[i] = value;
                gamma_g[i] = value;
                gamma_b[i] = value;
            }

            colorRampFill(gamma_r, gamma_g, gamma_b, ramp_size, iTemperature);

            CGError error =
                CGSetDisplayTransferByTable(display, ramp_size,
                                gamma_r, gamma_g, gamma_b);
            if (error != kCGErrorSuccess) {
                free(gamma_ramps);
                return false;
            }

            free(gamma_ramps);
        }
    }

    return true;
}

// Color ramp fill:
void MacMonitor::colorRampFill(float *gamma_r, float *gamma_g, float *gamma_b,
             int iSize, int iTemperature)
{
    // Approximate white point:
    float white_point[3];
    float alpha = (iTemperature % 100) / 100.0;
    int temp_index = ((iTemperature - 1000) / 100)*3;
    interpolateColor(alpha, &blackbody_color[temp_index],
              &blackbody_color[temp_index+3], white_point);

    for (int i = 0; i < iSize; i++) {
        gamma_r[i] = Monitor::fillFunction(white_point, (double)gamma_r[i], 0);
        gamma_g[i] = Monitor::fillFunction(white_point, (double)gamma_g[i], 1);
        gamma_b[i] = Monitor::fillFunction(white_point, (double)gamma_b[i], 2);
    }
}
