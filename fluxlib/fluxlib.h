#ifndef FLUXLIB_H
#define FLUXLIB_H
#include "fluxlib_global.h"
#include <QColor>

class FLUXLIBSHARED_EXPORT Fluxlib
{
public:
    static QColor colorForTemperature(int temperature);
};

#endif // FLUXLIB_H
