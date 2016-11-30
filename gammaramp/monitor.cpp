#include "monitor.h"
#include <stdlib.h>
#include <math.h>
#include <qglobal.h>

// Constructor:
Monitor::Monitor() : m_dRedGamma(1.0),
    m_dGreenGamma(1.0), m_dBlueGamma(1.0),
    m_dBrightness(1.0), m_iCurrentTemperature(0)
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
