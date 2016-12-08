#include "monitor.h"
#include <stdlib.h>
#include <math.h>
#include <qglobal.h>
#include <QDebug>

#include <stdio.h>
#include <unistd.h>

#ifndef _WIN32
# if _POSIX_TIMERS > 0
#  include <time.h>
# else
#  include <time.h>
#  include <sys/time.h>
# endif
#else
# include <windows.h>
#endif

// Constructor:
Monitor::Monitor() : m_dRedGamma(1.0),
    m_dGreenGamma(1.0), m_dBlueGamma(1.0),
    m_dBrightness(0.9), m_iCurrentTemperature(0)
{

}

// Init:
bool Monitor::init()
{
    return false;
}

// Start:
bool Monitor::start()
{
    return false;
}

// Restore:
void Monitor::restore()
{
}

// Set temperature:
bool Monitor::setTemperature(int iTemperature, bool bForce)
{
    Q_UNUSED(iTemperature);
    Q_UNUSED(bForce);
    return false;
}

// Interpolate color:
void Monitor::interpolateColor(float a, const float *c1, const float *c2, float *c)
{
    c[0] = (1.0-a)*c1[0] + a*c2[0];
    c[1] = (1.0-a)*c1[1] + a*c2[1];
    c[2] = (1.0-a)*c1[2] + a*c2[2];
}

// Fill function:
double Monitor::fillFunction(float *pWhitePoint, double dScaledGamma, int iIndex)
{
    double toUse = m_dRedGamma;
    if (iIndex == 1)
        toUse = m_dGreenGamma;
    if (iIndex == 2)
        toUse = m_dBlueGamma;
    if (toUse > 0)
        return pow((dScaledGamma) * m_dBrightness * pWhitePoint[iIndex], 1.0/toUse);
    return 1.0;
}

// Set brightness:
void Monitor::setBrightness(double dBrightness)
{
    m_dBrightness = dBrightness;
    setTemperature(m_iCurrentTemperature, true);
}

// Return brightness:
double Monitor::brightness() const
{
    return m_dBrightness;
}

// Set red gamma:
void Monitor::setRedGamma(double dRedGamma)
{
    m_dRedGamma = dRedGamma;
    setTemperature(m_iCurrentTemperature, true);
}

// Return red gamma:
double Monitor::redGamma() const
{
    return m_dRedGamma;
}

// Set green gamma:
void Monitor::setGreenGamma(double dGreenGamma)
{
    m_dGreenGamma = dGreenGamma;
    setTemperature(m_iCurrentTemperature, true);
}

// Return green gamma:
double Monitor::greenGamma() const
{
    return m_dGreenGamma;
}

// Set blue gamma:
void Monitor::setBlueGamma(double dBlueGamma)
{
    m_dBlueGamma = dBlueGamma;
    setTemperature(m_iCurrentTemperature, true);
}

// Return blue gamma:
double Monitor::blueGamma() const
{
    return m_dBlueGamma;
}

// Return elevation from hour angle:
double Monitor::elevationFromHourAngle(double dLat, double dDecl, double dHa)
{
    return asin(cos(dHa)*cos(RAD(dLat))*cos(dDecl) +
                sin(RAD(dLat))*sin(dDecl));
}

// Julian day from Julian centuries since J2000.0:
double Monitor::jdFromJCent(double dTime)
{
    return 36525.0*dTime + 2451545.0;
}

//Julian centuries since J2000.0 from Julian day::
double Monitor::jcentFromJd(double jd)
{
    return (jd - 2451545.0) / 36525.0;
}

// Mean obliquity of the ecliptic
// t: Julian centuries since J2000.0
// Return: Mean obliquity in radians
double Monitor::meanEclipticObliquity(double dTime)
{
    double dSec = 21.448 - dTime*(46.815 + dTime*(0.00059 - dTime*0.001813));
    return RAD(23.0 + (26.0 + (dSec/60.0))/60.0);
}

// Corrected obliquity of the ecliptic.
// t: Julian centuries since J2000.0
// Return: Currected obliquity in radians
double Monitor::obliquityCorr(double dTime)
{
    double e_0 = meanEclipticObliquity(dTime);
    double dOmega = 125.04 - dTime*1934.136;
    return RAD(DEG(e_0) + 0.00256*cos(RAD(dOmega)));
}

// Geometric mean longitude of the sun.
// t: Julian centuries since J2000.0
// Return: Geometric mean logitude in radians.
double Monitor::sunGeomMeanLon(double dTime)
{
    return RAD(fmod(280.46646 + dTime*(36000.76983 + dTime*0.0003032), 360));
}

// Geometric mean anomaly of the sun.
// t: Julian centuries since J2000.0
// Return: Geometric mean anomaly in radians.
double Monitor::sunGeomMeanAnomaly(double dTime)
{
    return RAD(357.52911 + dTime*(35999.05029 - dTime*0.0001537));
}

// Eccentricity of earth orbit.
// t: Julian centuries since J2000.0
// Return: Eccentricity (unitless).
double Monitor::earthOrbitEccentricity(double dTime)
{
    return 0.016708634 - dTime*(0.000042037 + dTime*0.0000001267);
}

// Difference between true solar time and mean solar time.
// t: Julian centuries since J2000.0
// Return: Difference in minutes.
double Monitor::equationOfTime(double dTime)
{
    double dEpsilon = obliquityCorr(dTime);
    double l_0 = sunGeomMeanLon(dTime);
    double e = earthOrbitEccentricity(dTime);
    double m = sunGeomMeanAnomaly(dTime);
    double y = pow(tan(dEpsilon/2.0), 2.0);

    double eq_time = y*sin(2*l_0) - 2*e*sin(m) +
            4*e*y*sin(m)*cos(2*l_0) -
            0.5*y*y*sin(4*l_0) -
            1.25*e*e*sin(2*m);
    return 4*DEG(eq_time);
}

// Equation of center of the sun.
// t: Julian centuries since J2000.0
// Return: Center(?) in radians.
double Monitor::sunEquationOfCenter(double dTime)
{
    // Use the first three terms of the equation..
    double m = sunGeomMeanAnomaly(dTime);
    double c = sin(m)*(1.914602 - dTime*(0.004817 + 0.000014*dTime)) +
            sin(2*m)*(0.019993 - 0.000101*dTime) +
            sin(3*m)*0.000289;
    return RAD(c);
}

// True longitude of the sun.
// t: Julian centuries since J2000.0
// Return: True longitude in radians.
double Monitor::sunTrueLon(double dTime)
{
    double l_0 = sunGeomMeanLon(dTime);
    double c = sunEquationOfCenter(dTime);
    return l_0 + c;
}

// Apparent longitude of the sun. (Right ascension).
// t: Julian centuries since J2000.0
// Return: Apparent longitude in radians.
double Monitor::sunApparentLon(double dTime)
{
    double o = sunTrueLon(dTime);
    return RAD(DEG(o) - 0.00569 - 0.00478*sin(RAD(125.04 - 1934.136*dTime)));
}

// Declination of the sun.
// t: Julian centuries since J2000.0
// Return: Declination in radians.
double Monitor::solarDeclination(double dTime)
{
    double e = obliquityCorr(dTime);
    double lambda = sunApparentLon(dTime);
    return asin(sin(e)*sin(lambda));
}

// Return solar elevation from time:
double Monitor::solarElevationFromTime(double t, double lat, double lon)
{
    // Minutes from midnight.
    double jd = jdFromJCent(t);
    double offset = (jd - round(jd) - 0.5)*1440.0;

    double eq_time = equationOfTime(t);
    double ha = RAD((720 - offset - eq_time)/4 - lon);
    double decl = solarDeclination(t);
    return elevationFromHourAngle(lat, decl, ha);
}

// Julian day from unix epoch:
double Monitor::jdFromEpoch(double dTime)
{
    return (dTime / 86400.0) + 2440587.5;
}

// Return solar elevation:
double Monitor::solarElevation(double date, double lat, double lon)
{
    double jd = jdFromEpoch(date);
    return DEG(solarElevationFromTime(jcentFromJd(jd), lat, lon));
}

// Return current time in T as the number of seconds since the epoch:
int Monitor::systemTimeGetTime(double *dTime)
{
#if defined(_WIN32) // Windows.
    FILETIME now;
    ULARGE_INTEGER i;
    GetSystemTimeAsFileTime(&now);
    i.LowPart = now.dwLowDateTime;
    i.HighPart = now.dwHighDateTime;

    // FILETIME is tenths of microseconds since 1601-01-01 UTC.
    *dTime = (i.QuadPart / 10000000.0) - 11644473600.0;
#elif _POSIX_TIMERS > 0 // POSIX timers.
    struct timespec now;
    int r = clock_gettime(CLOCK_REALTIME, &now);
    if (r < 0) {
        perror("clock_gettime");
        return -1;
    }

    *dTime = now.tv_sec + (now.tv_nsec / 1000000000.0);
#else // other platforms.
    struct timeval now;
    int r = gettimeofday(&now, NULL);
    if (r < 0) {
        perror("gettimeofday");
        return -1;
    }

    *dTime = now.tv_sec + (now.tv_usec / 1000000.0);
#endif

    return 0;
}

// Interpolate color setting structs based on solar elevation:
void Monitor::interpolateColorSettings(const transitionScheme *transition,
                                       double elevation,
                                       color_setting_t *result)
{
    const color_setting_t *day = &transition->day;
    const color_setting_t *night = &transition->night;

    double alpha = (transition->low - elevation) /
            (transition->low - transition->high);
    alpha = CLAMP(0.0, alpha, 1.0);

    result->temperature = (1.0-alpha)*night->temperature +
            alpha*day->temperature;
    result->brightness = (1.0-alpha)*night->brightness +
            alpha*day->brightness;
    for (int i = 0; i < 3; i++) {
        result->gamma[i] = (1.0-alpha)*night->gamma[i] +
                alpha*day->gamma[i];
    }
}

// Determine which period we are currently in:
period_t Monitor::getPeriod(const transitionScheme *transition,
                            double elevation)
{
    if (elevation < transition->low) {
        return PERIOD_NIGHT;
    } else if (elevation < transition->high) {
        return PERIOD_TRANSITION;
    } else {
        return PERIOD_DAYTIME;
    }
}

// Determine how far through the transition we are:
double Monitor::getTransitionProgress(const transitionScheme *transition,
                                      double elevation)
{
    if (elevation < transition->low) {
        return 0.0;
    } else if (elevation < transition->high) {
        return (transition->low - elevation) /
                (transition->low - transition->high);
    } else {
        return 1.0;
    }
}

// Print verbose description of the given period:
void Monitor::printPeriod(period_t period, double transition)
{
    switch (period) {
        case PERIOD_NONE: {
            qDebug() << "NONE";
            break;
        }
        case PERIOD_NIGHT: {
            qDebug() << "NIGHT";
            break;
        }
        case PERIOD_DAYTIME: {
            qDebug() << "DAYTIME";
            break;
        }
        case PERIOD_TRANSITION: {
            qDebug() << "TRANSITION: " << transition;
            break;
        }
    }
}

// Run continual mode loop
// This is the main loop of the continual mode which keeps track of the
// current time and continuously updates the screen to the appropriate
// color temperature..
int Monitor::runContinualMode(const geoLoc *loc,
                              const transitionScheme *scheme,
                              int transition)
{
    // Initialize:
    start();

    // Make an initial transition from 6500K.
    int short_trans_delta = -1;
    int short_trans_len = 10;
    int verbose = 1;

    // Amount of adjustment to apply. At zero the color
    // temperature will be exactly as calculated, and at one it
    // will be exactly 6500K.
    double adjustment_alpha = 1.0;

    // Save previous colors so we can avoid
    // printing status updates if the values
    // did not change..
    period_t prev_period = PERIOD_NONE;
    color_setting_t prev_interp =
    { -1, { NAN, NAN, NAN }, NAN };

    // Continuously adjust color temperature:
    int done = 0;
    int disabled = 0;
    bool disable = false;
    while (1) {
        // Check to see if disable signal was caught.
        if (disable) {
            short_trans_len = 2;
            if (!disabled) {
                // Transition to disabled state.
                short_trans_delta = 1;
            } else {
                // Transition back to enabled.
                short_trans_delta = -1;
            }
            disabled = !disabled;
            disable = 0;
        }

        // Check to see if exit signal was caught.
        bool exiting = false;
        if (exiting) {
            if (done) {
                // On second signal stop the ongoing
                // transition.
                short_trans_delta = 0;
                adjustment_alpha = 0.0;
            } else {
                if (!disabled) {
                    // Make a short transition
                    // back to 6500K.
                    short_trans_delta = 1;
                    short_trans_len = 2;
                }

                done = 1;
            }
            exiting = 0;
        }

        // Read timestamp.
        double now;
        int r = Monitor::systemTimeGetTime(&now);
        if (r < 0) {
            qDebug() << "Unable to read system time";
            continue;
        }

        // Skip over transition if transitions are disabled.
        int set_adjustments = 0;
        if (!transition) {
            if (short_trans_delta) {
                adjustment_alpha = short_trans_delta < 0 ?
                            0.0 : 1.0;
                short_trans_delta = 0;
                set_adjustments = 1;
            }
        }

        // Current angular elevation of the sun.
        double elevation = Monitor::solarElevation(now, loc->lat, loc->lon);

        // Use elevation of sun to set color temperature.
        color_setting_t interp;
        Monitor::interpolateColorSettings(scheme, elevation, &interp);

        // Print period if it changed during this update,
        // or if we are in transition. In transition we
        // print the progress, so we always print it in
        // that case..
        period_t period = Monitor::getPeriod(scheme, elevation);
        if (verbose && (period != prev_period ||
                        period == PERIOD_TRANSITION)) {
            double transition =
                    Monitor::getTransitionProgress(scheme,
                                                   elevation);
            Monitor::printPeriod(period, transition);
        }

        // Ongoing short transition.
        if (short_trans_delta) {
            // Calculate alpha.
            adjustment_alpha += short_trans_delta * 0.1 /
                    (float)short_trans_len;

            // Stop transition when done.
            if (adjustment_alpha <= 0.0 ||
                    adjustment_alpha >= 1.0) {
                short_trans_delta = 0;
            }

            // Clamp alpha value.
            adjustment_alpha = CLAMP(0.0, adjustment_alpha, 1.0);
        }

        // Interpolate between 6500K and calculated
        // temperature.
        interp.temperature = adjustment_alpha*6500 +
                (1.0-adjustment_alpha)*interp.temperature;

        interp.brightness = adjustment_alpha*1.0 +
                (1.0-adjustment_alpha)*interp.brightness;

        // Quit loop when done.
        if (done && !short_trans_delta) {
            restore();
            return 0;
        }

        if (verbose) {
            if (interp.temperature !=
                    prev_interp.temperature) {
                qDebug() << "Color temperature: " << interp.temperature;
            }
            if (interp.brightness !=
                    prev_interp.brightness) {
                qDebug() << "Brightness: " << interp.brightness;
            }
        }

        // Adjust temperature.
        if (!disabled || short_trans_delta || set_adjustments) {
            int iTemperature = interp.temperature;
            m_dRedGamma = interp.gamma[0];
            m_dGreenGamma = interp.gamma[1];
            m_dBlueGamma = interp.gamma[2];
            m_dBrightness = (double)interp.brightness;

            if (iTemperature == 3500)
            {
                int x = 0;
            }
            bool r = setTemperature(iTemperature);
            if (!r) {
                qDebug() << "Temperature adjustement failed";
                return -1;
            }
        }

        // Save temperature as previous.
        prev_period = period;
        memcpy(&prev_interp, &interp, sizeof(color_setting_t));
    }

    // TO DO
    restore();

    return 0;
}
