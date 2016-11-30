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
    m_bPreserve = false;
    return true;
}

// Start:
bool MacMonitor::start()
{
    CGError error;
    uint32_t display_count;

    // Get display count:
    error = CGGetOnlineDisplayList(0, NULL, &display_count);
    if (error != kCGErrorSuccess) return false;

    CGDirectDisplayID* displays =
        malloc(sizeof(CGDirectDisplayID)*display_count);
    if (displays == NULL) {
        qDebug() << "Malloc error";
        return false;
    }

    // Get list of displays:
    error = CGGetOnlineDisplayList(display_count, displays,
                       &display_count);
    if (error != kCGErrorSuccess) {
        free(displays);
        return false;
    }

    // Save gamma ramps for all displays in display state:
    display_count = 1;
    for (int i = 0; i < display_count; i++) {
        CGDirectDisplayID display = displays[i].display;

        uint32_t ramp_size = CGDisplayGammaTableCapacity(display);
        if (ramp_size == 0) {
            qDebug() << "Gamma ramp size too small: " << ramp_size;
            return false;
        }

        displays[i].ramp_size = ramp_size;

        // Allocate space for saved ramps:
        m_pSavedRamps =
            malloc(3 * ramp_size * sizeof(float));
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
            uint32_t ramp_size = state->displays[display].ramp_size;

            // Create new gamma ramps:
            float *gamma_ramps = malloc(3*ramp_size*sizeof(float));
            if (gamma_ramps == NULL) {
                qDebug() << "Malloc error";
                return false;
            }

            float *gamma_r = &gamma_ramps[0*ramp_size];
            float *gamma_g = &gamma_ramps[1*ramp_size];
            float *gamma_b = &gamma_ramps[2*ramp_size];

            if (state->preserve) {
                // Initialize gamma ramps from saved state:
                memcpy(gamma_ramps, m_pSavedRamps,
                       3*ramp_size*sizeof(float));
            } else {
                // Initialize gamma ramps to pure state:
                for (int i = 0; i < ramp_size; i++) {
                    float value = (double)i/ramp_size;
                    gamma_r[i] = value;
                    gamma_g[i] = value;
                    gamma_b[i] = value;
                }
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
    interpolate_color(alpha, &blackbody_color[temp_index],
              &blackbody_color[temp_index+3], white_point);

    for (int i = 0; i < iSize; i++) {
        gamma_r[i] = fillFunction(white_point, (double)gamma_r[i], 0);
        gamma_g[i] = fillFunction(white_point, (double)gamma_g[i], 1);
        gamma_b[i] = fillFunction(white_point, (double)gamma_b[i], 2);
    }
}
