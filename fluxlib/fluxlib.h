#ifndef FLUXLIB_H
#define FLUXLIB_H
#include "fluxlib_global.h"
#include <QColor>

class FLUXLIBSHARED_EXPORT Fluxlib
{
public:
    // Return color for temperature:
    static QColor colorForTemperature(int iTemperature);
    static QColor colorForTemperature1(int kelvin);
};

#endif // FLUXLIB_H
