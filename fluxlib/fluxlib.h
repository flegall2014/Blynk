#ifndef FLUXLIB_H
#define FLUXLIB_H
#include "fluxlib_global.h"
#include <QColor>

class FLUXLIBSHARED_EXPORT Fluxlib
{
public:
    // Return color for temperature:
    static QColor colorForTemperature(int iTemperature);
};

#endif // FLUXLIB_H
